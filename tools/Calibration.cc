/*!
*
* \file Calibration.cc
* \brief Calibration class, calibration of the hardware
* \author Lorenzo BIDEGAIN
* \date 13/08/14
*
* Support : mail to : lorenzo.bidegain@cern.ch
*
*/

#include "Calibration.h"

Calibration::Calibration(std::string pResultfilepath){
	// if(fResultFile != NULL) delete fResultFile;
	// else{
		fResultFile = TFile::Open(pResultfilepath.c_str(),"RECREATE");
	// }
		// InitialiseTestGroup();
}

Calibration::~Calibration(){

			fResultFile->Write();
			fResultFile->Close();
}

void Calibration::InitialiseTestGroup()
{
    // Iterating over the Shelves
    for (auto cShelve:fShelveVec)
    {
        for(auto cBoard:(cShelve)->fBoardVector)
        {
            for(auto cFe : cBoard.fModuleVector)
            {
                for(auto cCbc : cFe.fCbcVector)
                {
                  // std::cout<<"Cbc: "<<cCbc<<"created!"<<std::endl;
                	// std::cout << uint32_t((cShelve)->getShelveId()) << " " << uint32_t(cBoard.getBeId()) << " " << uint32_t(cFe.getFeId()) << " " << uint32_t(cCbc.getCbcId()) << std::endl;
                   ConstructTestGroup((cShelve)->getShelveId(),cBoard.getBeId(), cFe.getFeId(), cCbc.getCbcId());
                }
            }
        }
    }
}


void Calibration::ConstructTestGroup(uint8_t pShelveId, uint8_t pBeId, uint8_t pFeId, uint8_t pCbcId)
{
	for(uint8_t lgroup=0; lgroup<8; lgroup++)
	{
		TestGroup fTestgroup(pShelveId, pBeId, pFeId, pCbcId, lgroup);

		std::vector<Channel> cChannelVect;

        // In the CBC Registers, Channels start with 1
		for(uint8_t lchannel=0; lchannel<16; lchannel++)
		{

			if (lchannel*16+lgroup*2 < 254)
			{
				Channel cChannel(pBeId,pFeId,pCbcId, lchannel*16+lgroup*2 +1 );
				// std::cout<<"		Channel: "<<int(lchannel*16+lgroup*2)<<"created!"<<std::endl;
				cChannelVect.push_back(cChannel);
			}

			if ((lchannel*16+lgroup*2) +1 < 254)
			{
				Channel cChannel(pBeId,pFeId,pCbcId, (lchannel*16+lgroup*2) +2 );
				// std::cout<<"		Channel: "<<int( (lchannel*16+lgroup*2) +1)<<"created!"<<std::endl;
				cChannelVect.push_back(cChannel);
			}

		}

		// std::cout << "Shelve " << uint32_t(fTestgroup.fShelveId) << " Be " << uint32_t(fTestgroup.fBeId) << " FE " << uint32_t(fTestgroup.fFeId) << " Cbc " << uint32_t(fTestgroup.fCbcId) << " Group " << uint32_t(fTestgroup.fGroupId) << std::endl;

		TestGroupGraph cTestGroupGraph(pBeId, pFeId, pCbcId, lgroup);


		fTestGroupMap[fTestgroup]=cChannelVect;
		fTestGroupGraphMap[fTestgroup]=cTestGroupGraph;
		// std::cout<<"	Group: "<<int(lgroup)<<" created @ " << &fTestGroupMap[fTestgroup] <<std::endl;

	}
}


void Calibration::OffsetScan(){

	// Read values from Settings
	uint32_t cEventsperVcth = fSettingsMap.find("Nevents")->second;
	bool cHoleMode = fSettingsMap.find("HoleMode")->second;
	uint8_t cTargetVcth = fSettingsMap.find("TargetVcth")->second;

	for(auto& cShelve : fShelveVec)
	{
		// Iterating over the Boards
		for(BeBoard& board : cShelve->fBoardVector)
		{

			setGlobalReg(board, "Vplus", 110);

			// Now loop over test groups, enable them, loop over Vcth, read & analyze data, fill histograms & disable test groups again
			for(uint8_t cGroupId = 0; cGroupId < 8; cGroupId++)
			{
				// set Offsets to 0 initially
				ToggleTestGroup(board, cGroupId, cHoleMode, false);

				std::cout << BOLDYELLOW << "Looping over OffsetBits ... " << RESET << std::endl;

				// loop over offsets bitwise
				// for(int cTargetBit = 0x07; cTargetBit >= 0x00; cTargetBit-- ){
				uint8_t cTargetBit = 0x07;
				do{
					//Set Offset Target bit
					uint32_t cTotalChannels = SetOffsetTargetBitTestGroup(board, cGroupId, cHoleMode, cTargetBit, cTargetVcth);
				
					// Now set Vplus to the correct value for all Cbc's connected to the current board
					initializeSCurves(board, cGroupId, cTargetBit, "Offset_TargetBit");

					measureSCurves(board, cGroupId, cEventsperVcth, cTotalChannels, cHoleMode);

					// the bool at the end toggles the printing of the SCurves
					processSCurvesOffset(board, cGroupId, cEventsperVcth, cTargetVcth, cTargetBit, "Offset_TargetBit", cHoleMode, true);

				} while(cTargetBit--);// End of TargetBitLoop

				// Disbale the current TestGroup
				ToggleTestGroup(board, cGroupId, cHoleMode, false);

			} // End of TestGroup Loop
		} // End of Be Loop
	} // End of the Shelve Loop
}

uint32_t Calibration::SetOffsetTargetBitTestGroup(BeBoard& pBoard, uint8_t pGroupId, bool pHoleMode, uint8_t pTargetBit, uint8_t pTargetVcth){

	uint32_t cTotalNChannels = 0;
	for(auto& cGroupIt : fTestGroupMap)
	{
		// check if the Group belongs to the right Shelve, BE and if it is the right group
		if(cGroupIt.first.fShelveId == pBoard.getShelveId() && cGroupIt.first.fBeId == pBoard.getBeId() && cGroupIt.first.fGroupId == pGroupId)
		{
			// Iterate through the channels and set Offset to 0x50
			// Vector of pairs for the write MultiReg
			std::vector< std::pair< std::string, uint8_t > > cRegVec;

			for(Channel& cChannel : cGroupIt.second)
			{

				TString cRegName = Form("Channel%03d",cChannel.fChannelId);
				uint8_t cOffset = cChannel.getOffset(); 
				// uint8_t cOffset = pBoard.getModule(cGroupIt.first.fFeId)->getCbc(cGroupIt.first.fCbcId)->getReg(cRegName.Data());

				if(pHoleMode){
					// cOffset = 0x00;
				// independently of the CBC logic, just toggle the bit
					cOffset |= (1<<pTargetBit);
				}
				else{
				// 	cOffset = 0xFF;
					cOffset &= ~(1<<pTargetBit);
				}
				
				std::pair<std::string, uint8_t> cRegPair = std::make_pair(cRegName.Data(), cOffset);
				cRegVec.push_back(cRegPair);

				// Update the Channel Object
				cChannel.setOffset(cOffset);

				cTotalNChannels++;
			}

			if(!cRegVec.empty()) fCbcInterface->WriteCbcMultReg(pBoard.getModule(cGroupIt.first.fFeId)->getCbc(cGroupIt.first.fCbcId),cRegVec);
		}
	}
	std::cout << GREEN << "Flipping Bit  " << YELLOW <<  uint32_t(pTargetBit) << GREEN << " on Test Group " << YELLOW <<  uint32_t(pGroupId) << RESET << std::endl;
	return cTotalNChannels;
}



void Calibration::processSCurvesOffset(BeBoard& pBoard, uint8_t pGroupId, uint32_t pEventsperVcth, uint8_t pTargetVcth, uint8_t pTargetBit, TString pParameter, bool pHoleMode, bool pDoDraw){
	// Here Loop over all FE's Cbc's, Test Groups & Channels to fill Histograms
   for(auto& cGroupIt : fTestGroupMap)
   {

   	// check if the Group belongs to the right Shelve, BE and if it is the right group
   	if(cGroupIt.first.fShelveId == pBoard.getShelveId() && cGroupIt.first.fBeId == pBoard.getBeId() && cGroupIt.first.fGroupId == pGroupId)
   	{

   		TCanvas* cSCurveCanvas;

   		if(pDoDraw){
	    		TString cSCurveCanvasName = Form("SCurves_FE%d_CBC%d_TestGroup%d", cGroupIt.first.fFeId, cGroupIt.first.fCbcId, pGroupId);
	    		cSCurveCanvas = (TCanvas*) gROOT->FindObject(cSCurveCanvasName);
	    		if(cSCurveCanvas) delete cSCurveCanvas;
	    		cSCurveCanvas = new TCanvas(cSCurveCanvasName,cSCurveCanvasName);
	    		cSCurveCanvas->cd();
	    	}

	    std::vector< std::pair< std::string, uint8_t > > cRegVec;

   		bool cFirst = true;
   		TString cOption;
   		uint32_t cChannelCounter = 1;
			
   		for(Channel& cChannel : cGroupIt.second)
   		{
				cChannel.fitHist(pEventsperVcth, pHoleMode, pTargetBit, pParameter, fResultFile);

				if(pDoDraw){
					gStyle->SetOptStat(00000000000);
					// Drawing SCurves of current test Group
					if(cFirst) {
						cOption = "P0";
						cFirst = false;
					}
					else cOption = "P0 same";

					cChannel.fScurve->SetMarkerColor(cChannelCounter);
					cChannel.fScurve->Draw(cOption);
					cChannel.fFit->Draw("same");
				}

				// here we need to check if the SCurve midpoint(cChannel.getPedestal()) with this offset target bit is > or < than pTargetVcth and then leave the bit up or flip it back down

				std::cout << int(pTargetVcth) << " " << cChannel.getPedestal() << std::endl;

				if(pHoleMode && int(cChannel.getPedestal()) > pTargetVcth){
					uint8_t cOffset = cChannel.getOffset();
					// uint8_t cOffset = pBoard.getModule(cGroupIt.first.fFeId)->getCbc(cGroupIt.first.fCbcId)->getReg(cRegName.Data());

					cOffset &= ~(1<<pTargetBit);

					TString cRegName = Form("Channel%03d",cChannel.fChannelId);
					std::pair<std::string, uint8_t> cRegPair = std::make_pair(cRegName.Data(), cOffset);
					cRegVec.push_back(cRegPair);
					cChannel.setOffset(cOffset);
				}
				else if(!pHoleMode && int(cChannel.getPedestal()) < pTargetVcth){
					uint8_t cOffset = cChannel.getOffset();
					// uint8_t cOffset = pBoard.getModule(cGroupIt.first.fFeId)->getCbc(cGroupIt.first.fCbcId)->getReg(cRegName.Data());

					cOffset |= (1<<pTargetBit);
					// toggle bit 
					// cOffset ^= (1<<pTargetBit);

					TString cRegName = Form("Channel%03d",cChannel.fChannelId);
					std::pair<std::string, uint8_t> cRegPair = std::make_pair(cRegName.Data(), cOffset);
					cRegVec.push_back(cRegPair);
					cChannel.setOffset(cOffset);
				}
			}

			if(!cRegVec.empty()) fCbcInterface->WriteCbcMultReg(pBoard.getModule(cGroupIt.first.fFeId)->getCbc(cGroupIt.first.fCbcId),cRegVec);
			cSCurveCanvas->Update();
			cChannelCounter++;
		}
	}
	std::cout << "Processed SCurves for Target Bit " << GREEN <<  uint32_t(pTargetBit) << RESET << std::endl;
}


void Calibration::VplusScan(){

	// for 4 different values of Vplus, loop over all Test Groups on all Cbc's simultaniously:
	// 1. enable test groups on all Cbc's on 1 FE
	// 2. Loop over Vcth Range and set Vcth on all Cbc's
	// 3. Run and Take Data for all Cbc's on 1 FE
	// 4. Get Event and again loop over all Cbc's on 1 FE and analyze the data

	fVplusValues.push_back(0x40);
	fVplusValues.push_back(0x50);
	// fVplusValues.push_back(0x60);
	// fVplusValues.push_back(0x70);

	// Read values from Settings
	uint32_t cEventsperVcth = fSettingsMap.find("Nevents")->second;
	bool cHoleMode = fSettingsMap.find("HoleMode")->second;
	uint8_t cTargetVcth = fSettingsMap.find("TargetVcth")->second;

	for(auto& cShelve : fShelveVec)
	{
		// Iterating over the Boards
		for(BeBoard& board : cShelve->fBoardVector)
		{

			// Now loop over test groups, enable them, loop over Vcth, read & analyze data, fill histograms & disable test groups again
			for(uint8_t cGroupId = 0; cGroupId < 8; cGroupId++)
			{
				// Enable the current TestGroup
				uint32_t cTotalChannels = ToggleTestGroup(board, cGroupId, cHoleMode, true);

				std::cout << BOLDYELLOW << "Scanning Vplus ... " << RESET << std::endl;
				for(uint8_t& cVplus : fVplusValues)
				{

					// Now set Vplus to the correct value for all Cbc's connected to the current board
					setGlobalReg(board, "Vplus", cVplus);

					initializeSCurves(board, cGroupId, cVplus, "Vplus");

					measureSCurves(board, cGroupId, cEventsperVcth, cTotalChannels, cHoleMode);
					// Now Fit the hists & fill the VplusVcth Graph & set the TestGroup Offset back to 0x00 or 0xFF
					processSCurves(board, cGroupId, cEventsperVcth, cVplus, "Vplus", cHoleMode, true);

				} // End of Vplus Loop

				// Disbale the current TestGroup
				ToggleTestGroup(board, cGroupId, cHoleMode, false);

			} // End of TestGroup Loop

			// Fit the Thing
			FitVplusVcth(board, cTargetVcth, true);
		} // End of Be Loop
	} // End of the Shelve Loop
}



void Calibration::FitVplusVcth(BeBoard& pBoard, uint8_t pTargetVcth,  bool pDoDraw){

	for(auto cFe : pBoard.fModuleVector)
	{
		for(Cbc& cCbc : cFe.fCbcVector)
		{
			TString multigraphname = Form("VplusVcth_Be%d_Fe%d_Cbc%d", pBoard.getBeId(), cFe.getFeId(), cCbc.getCbcId());
			TMultiGraph* cVplusVcthMultiGraph = new TMultiGraph(multigraphname,Form("Vplus vs. Vcth; Vcth; Vplus"));
			TCanvas* cVplusVcthCanvas;

			if (pDoDraw){
				TString canvasname = Form("Be%d_Fe%d_Cbc%d_VplusVcth", pBoard.getBeId(), cFe.getFeId(), cCbc.getCbcId());
				cVplusVcthCanvas = (TCanvas*) gROOT->FindObject(canvasname);
				if (cVplusVcthCanvas != NULL) delete cVplusVcthCanvas;
				cVplusVcthCanvas = new TCanvas(canvasname, canvasname);
				cVplusVcthCanvas->cd();
			}

			for(uint8_t cGroupId = 0; cGroupId < 8; cGroupId++)
			{

				// Create a test Group to find the corresponding in the map
				TestGroup cTempTestGroup(pBoard.getShelveId(), pBoard.getBeId(), cFe.getFeId(), cCbc.getCbcId(), cGroupId);

				// Find it!
				TestGroupGraphMap::iterator cGroup = fTestGroupGraphMap.find(cTempTestGroup);
				if (cGroup != fTestGroupGraphMap.end())
				{
					cGroup->second.fVplusVcthGraph->SetMarkerStyle(8);
					cGroup->second.fVplusVcthGraph->SetMarkerColor(cGroupId+1);
					cVplusVcthMultiGraph->Add(cGroup->second.fVplusVcthGraph);
				}
				else std::cout << BOLDRED << "This test group (BE" << int(pBoard.getBeId()) << " FE" << int(cFe.getFeId()) << " CBC" << int(cCbc.getCbcId()) << " Group" << int(cGroupId) << ") was not found!" << RESET << std::endl;
			}

			// All Points in the Multigraph; Fit it per Cbc, draw it Per Cbc
			// TF1* cVplusVcthFit = new TF1("VplusVcthFit","pol1",0,0xFF);
			TFitResultPtr cVplusVcthFitResult = cVplusVcthMultiGraph->Fit("pol1", "Q+");

			if (pDoDraw){
				cVplusVcthMultiGraph->Draw("AP");
				// cVplusVcthFit->Draw("same");
				gPad->Modified();
				cVplusVcthMultiGraph->GetXaxis()->SetLimits(0,255);
				cVplusVcthMultiGraph->SetMinimum(0);
				cVplusVcthMultiGraph->SetMaximum(255);
				cVplusVcthCanvas->Update();
				cVplusVcthMultiGraph->Write(cVplusVcthMultiGraph->GetName(),TObject::kOverwrite);
				fResultFile->Flush();
			}
			// Get the right Vplus setting & write to the Cbc
			// uint8_t cVplusResult = (uint8_t) ((int) cVplusVcthFit->Eval(pTargetVcth));

			// fCbcInterface->WriteCbcReg(cModule.getCbc(cCbc.getCbcId()),"Vplus",cVplusResult);
			// std::cout << "Vplus Setting for Be " << (int)cBoard.getBeId() << " Fe " << (int)cModule.getFeId() << " Cbc " << (int)cCbc.getCbcId() << " : " << int(cVplusResult) << std::endl;

		} // End of Cbc Loop
	} // End of Fe Loop
}

void Calibration::setGlobalReg(BeBoard& pBoard, std::string pRegName, uint8_t pRegValue){
	// Set 1 Register on all Cbcs connected to 1 BeBoard
	// BeBoard* cBoard = fShelveVec.at(pShelveId)->getBoard(pBeId);

	for(auto cFe : pBoard.fModuleVector)
	{
		for(Cbc& cCbc : cFe.fCbcVector)
		{
			fCbcInterface->WriteCbcReg(&cCbc, pRegName,pRegValue);
		}
	}
	if(pRegName != "VCth") std::cout << "Setting " << RED << pRegName << RESET << " to Value " << GREEN << (int)pRegValue << RESET << " on all CBCs connected to Be " << YELLOW << int(pBoard.getBeId()) << RESET << std::endl;
}

void Calibration::initializeSCurves(BeBoard& pBoard, uint8_t pGroupId, uint8_t pValue, TString pParameter){

	for(auto& cGroupIt : fTestGroupMap)
	{
		// check if the Group belongs to the right Shelve, BE and if it is the right group
		if(cGroupIt.first.fShelveId == pBoard.getShelveId() && cGroupIt.first.fBeId == pBoard.getBeId() && cGroupIt.first.fGroupId == pGroupId)
		{
			for(Channel& cChannel : cGroupIt.second) cChannel.initializeHist(pValue, pParameter);
		}
	}
	std::cout << "Initialized SCurves for " << pParameter << " = " << int(pValue) << " and Test group " << GREEN <<  uint32_t(pGroupId) << RESET << " on all Cbc's connected to Be " << RED <<  uint32_t(pBoard.getBeId()) << RESET << std::endl;
}

void Calibration::measureSCurves( BeBoard& pBoard, uint8_t pGroupId, uint32_t pEventsperVcth, uint32_t pTotalChannels, bool pHoleMode){

	// This is a smart Loop that scans the necessary Vcth range

	int cVcthMin(0x00), cVcthMax(0xFF);
	int cDoubleVcth = 0;
	int cVcth = pHoleMode ? cVcthMax : cVcthMin;
	int cStep = pHoleMode ? -10 : 10;
	bool cNonZero = false;
	uint32_t cAllOne = 0;

	std::cout << BOLDYELLOW << "Scanning VCth ... " << RESET << std::endl;
	while(0x00 <= cVcth && cVcth <= 0xFF)
	{
		if(cVcth == cDoubleVcth){
			cVcth +=  cStep;
		    continue;
		}

		// Set current Vcth value on all Cbc's of the current board
    	setGlobalReg(pBoard, "VCth", cVcth);

		uint32_t cN = 0;
		uint32_t cNthAcq = 0;
		uint32_t cTotalHits = 0;

		while( cN < pEventsperVcth)
		{
			Run(&pBoard, cNthAcq);

			const Event *cEvent = fBeBoardInterface->GetNextEvent(&pBoard);

			// Loop over Events from this Acquisition
		    while( cEvent )
		    {

		        if( cN == pEventsperVcth )
		        {
		            break;
		        }

		        uint32_t cNHits = fillScurveHists(pBoard, pGroupId, cVcth, cEvent);
		       	cTotalHits += cNHits;
		        cN++;

		        if(cN < pEventsperVcth ) cEvent = fBeBoardInterface->GetNextEvent(&pBoard);
		        else break;
			}
			cNthAcq++;
		} // End of Analyze Events of last Acquistion loop

		// This is the condition for some channels being different from 0
		if( cNonZero == false && cTotalHits != 0){
		// if( cNonZero == false && cTotalHits > 0.3 * pEventsperVcth * pTotalChannels){
			cDoubleVcth = cVcth;
			cNonZero = true;
			cVcth -= 2 * cStep;
			cStep /= 10;
			continue;
		}
		// This is the condition for all of the S-curves having reached 1
		if(cTotalHits == pEventsperVcth * pTotalChannels) cAllOne++;
		// if(cTotalHits >= 0.85 * pEventsperVcth * pTotalChannels) cAllOne++;
		if(cAllOne == 8) break;
		cVcth += cStep;
	}  // done looping over Vcth, Scurves created
}




void Calibration::processSCurves(BeBoard& pBoard, uint8_t pGroupId, uint32_t pEventsperVcth, uint8_t pValue, TString pParameter, bool pHoleMode, bool pDoDraw){
	// Here Loop over all FE's Cbc's, Test Groups & Channels to fill Histograms
    for(auto& cGroupIt : fTestGroupMap)
    {

    	// check if the Group belongs to the right Shelve, BE and if it is the right group
    	if(cGroupIt.first.fShelveId == pBoard.getShelveId() && cGroupIt.first.fBeId == pBoard.getBeId() && cGroupIt.first.fGroupId == pGroupId)
    	{

    		TCanvas* cSCurveCanvas;

    		if(pDoDraw){
	    		TString cSCurveCanvasName = Form("SCurves_FE%d_CBC%d_TestGroup%d", cGroupIt.first.fFeId, cGroupIt.first.fCbcId, pGroupId);
	    		cSCurveCanvas = (TCanvas*) gROOT->FindObject(cSCurveCanvasName);
	    		if(cSCurveCanvas) delete cSCurveCanvas;
	    		cSCurveCanvas = new TCanvas(cSCurveCanvasName,cSCurveCanvasName);
	    		cSCurveCanvas->cd();
	    	}

    		bool cFirst = true;
    		TString cOption;
    		uint32_t cChannelCounter = 1;
			
    		for(Channel& cChannel : cGroupIt.second)
    		{
				cChannel.fitHist(pEventsperVcth, pHoleMode, pValue, pParameter, fResultFile);

				if(pDoDraw){
					gStyle->SetOptStat(00000000000);
					// Drawing SCurves of current test Group
					if(cFirst) {
						cOption = "P0";
						cFirst = false;
					}
					else cOption = "P0 same";

					cChannel.fScurve->SetMarkerColor(cChannelCounter);
					cChannel.fScurve->Draw(cOption);
					cChannel.fFit->Draw("same");
				}

				// fill test Group TGraphErrors here
				TestGroupGraphMap::iterator cGraphIt = fTestGroupGraphMap.find(cGroupIt.first);
				if(cGraphIt != fTestGroupGraphMap.end()){

					cGraphIt->second.FillVplusVcthGraph(pValue, cChannel.getPedestal(), cChannel.getNoise());
					if(cChannel.getPedestal() == 0) std::cout << RED << "ERROR " << RESET << "The fit for Channel " << int(cChannel.fChannelId) << " CBC " << int(cChannel.fCbcId) << " FE " << int(cChannel.fFeId) << " did not work correctly!" << std::endl;
				}
				else std::cout << "The Graph for this test Group could not be found! There is a problem!" << std::endl;
				cChannelCounter++;
			}

			fResultFile->Flush();
			cSCurveCanvas->Update();
		}
	}
	std::cout << "Processed SCurves for Test group " << GREEN <<  uint32_t(pGroupId) << RESET << " on all Cbc's connected to Be " << RED <<  uint32_t(pBoard.getBeId()) << RESET << std::endl;
}

uint32_t Calibration::fillScurveHists(BeBoard& pBoard, uint8_t pGroupId, uint8_t pVcth, const Event* pEvent){
	// Here Loop over all FE's Cbc's, Test Groups & Channels to fill Histograms
	uint32_t cNHits = 0;

	for(auto& cGroupIt : fTestGroupMap)
	{
		// check if the Group belongs to the right Shelve, BE and if it is the right group
		if(cGroupIt.first.fShelveId == pBoard.getShelveId() && cGroupIt.first.fBeId == pBoard.getBeId() && cGroupIt.first.fGroupId == pGroupId)
		{
	        std::vector<bool> cDataBitVector = pEvent->DataBitVector(cGroupIt.first.fBeId, cGroupIt.first.fCbcId);

	        // Now loop over all channels in the TestGroup
			for(Channel& cChannel : cGroupIt.second)
			{
				// In the CBC, Channels start with 1, so decrement by 1 to have the right element from the data bit vector
	            if(cDataBitVector[cChannel.fChannelId-1])
	            {
	            	cChannel.fillHist(pVcth);
	                cNHits++;
	            }
	        } // Channel Loop
		} // If Statement
	}
	
	return cNHits;
}

uint32_t Calibration::ToggleTestGroup(BeBoard& pBoard, uint8_t pGroupId, bool pHoleMode, bool pEnable){

	uint32_t cTotalNChannels = 0;
	for(auto& cGroupIt : fTestGroupMap)
	{
		// check if the Group belongs to the right Shelve, BE and if it is the right group
		if(cGroupIt.first.fShelveId == pBoard.getShelveId() && cGroupIt.first.fBeId == pBoard.getBeId() && cGroupIt.first.fGroupId == pGroupId)
		{
			// Iterate through the channels and set Offset to 0x50
			// Vector of pairs for the write MultiReg
			std::vector< std::pair< std::string, uint8_t > > cRegVec;

			for(Channel& cChannel : cGroupIt.second)
			{
				// Write the offset to the Cbcs
				TString cRegName = Form("Channel%03d",cChannel.fChannelId);
				uint8_t cRegValue;
				if(pEnable) cRegValue = 0x50;
				else if(!pEnable && pHoleMode) cRegValue = 0x00;
				else cRegValue = 0xFF;
				std::pair<std::string, uint8_t> cRegPair = std::make_pair(cRegName.Data(), cRegValue);
				cRegVec.push_back(cRegPair);

				// Update the Channel Object
				if(pEnable) cChannel.setOffset(0x50);
				else if (!pEnable && pHoleMode) cChannel.setOffset(0x00);
				else cChannel.setOffset(0xFF);
				cTotalNChannels++;
			}

			fCbcInterface->WriteCbcMultReg(pBoard.getModule(cGroupIt.first.fFeId)->getCbc(cGroupIt.first.fCbcId),cRegVec);
		}
	}
	if(pEnable) std::cout << GREEN << "Enabled Test group " << YELLOW <<  uint32_t(pGroupId) << GREEN << " on all Cbc's connected to Be " << YELLOW <<  uint32_t(pBoard.getBeId()) << RESET << std::endl;
	else std::cout << RED << "Disabled Test group " << YELLOW <<  uint32_t(pGroupId) << RED << " on all Cbc's connected to Be " << YELLOW <<  uint32_t(pBoard.getBeId()) << RESET << std::endl;
	return cTotalNChannels;
}
