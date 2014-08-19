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


void Calibration::InitialiseTestGroup()
{

	
 //    InitializeSettings("../Setings/HWDescription.xml");
	// InitializeHw("../Setings/HWDescription.xml");

	uint32_t cMissedBoard, cMissedModule, cMissedCbc;

	for(uint32_t lShelve=0; lShelve<fShelveVec.size(); lShelve++)
	{
	  cMissedBoard = 0;

	  for(uint32_t lBoard=0; lBoard<fShelveVec[lShelve]->getNBoard(); lBoard++)
	  {
	    if(fShelveVec[lShelve]->getBoard(lBoard+cMissedBoard) == NULL)
	    {
	      lBoard--;
	      cMissedBoard++;
	    }

	    else
	    {
	      cMissedModule = 0;

              for(uint32_t lFe=0; lFe<fShelveVec[lShelve]->getBoard(lBoard+cMissedBoard)->getNFe(); lFe++)
	      {

		if(fShelveVec[lShelve]->getBoard(lBoard+cMissedBoard)->getModule(lFe+cMissedModule) == NULL)
		{
		  lFe--;
		  cMissedModule++;
		}

		else
		{
		  cMissedCbc = 0;

		  for(uint32_t lcbc=0; lcbc<fShelveVec[lShelve]->getBoard(lBoard+cMissedBoard)->getModule(lFe+cMissedModule)->getNCbc(); lcbc++)
		  {
			if(fShelveVec[lShelve]->getBoard(lBoard+cMissedBoard)->getModule(lBoard+cMissedModule)->getCbc(lcbc+cMissedCbc) == NULL)
		  {
			lcbc--;
		 	cMissedCbc++;
		  }

		  else
		  {

		  	std::cout<<"Cbc: "<<int(lcbc)<<"created!"<<std::endl;
			ConstructTestGroup(lBoard, lFe, lcbc);	

		  }

		  }
	        }
	      }
	    }
	  }
	}	
}

void Calibration::ConstructTestGroup(uint8_t pBeId, uint8_t FeId, uint8_t CbcId)
{
	for(uint8_t lgroup=0; lgroup<8; lgroup++)
	{
		TestGroup fTestgroup(pBeId,FeId,CbcId,lgroup);
				
		std::cout<<"	Group: "<<int(lgroup)<<"created!"<<std::endl;

		std::vector<Channel> cChannelVect;
		for(uint8_t lchannel=0; lchannel<16; lchannel++)
		{
					
			if (lchannel*16+lgroup*2 < 254)
			{
				Channel cChannel(pBeId,FeId,CbcId, lchannel*16+lgroup*2 );
				std::cout<<"		Channel: "<<int(lchannel*16+lgroup*2)<<"created!"<<std::endl;
				cChannelVect.push_back(cChannel);
			}

			if ((lchannel*16+lgroup*2) +1 < 254)
			{
				Channel cChannel(pBeId,FeId,CbcId, (lchannel*16+lgroup*2) +1 );
				std::cout<<"		Channel: "<<int( (lchannel*16+lgroup*2) +1)<<"created!"<<std::endl;
				cChannelVect.push_back(cChannel);
			}

		}

		fTestGroupMap[fTestgroup]=cChannelVect;
	}

}	


void Calibration::VplusScan(){

	fVplusValues.push_back(0x40);
	fVplusValues.push_back(0x50);
	fVplusValues.push_back(0x60);
	fVplusValues.push_back(0x70);

	uint32_t cEventsperVcth = fSettingsMap.find("Nevents")->second;
	bool cHoleMode = fSettingsMap.find("HoleMode")->second;
	uint8_t cTargetVcth = fSettingsMap.find("TargetVcth")->second;

	// Loop over 4 values of Vplus for the Vplus vs. Pedestal (Vcth) graph
	for(std::vector<uint8_t>::iterator cVplus = fVplusValues.begin(); cVplus != fVplusValues.end(); cVplus++)
	{
		for(std::vector<Shelve*>::iterator cShelve = fShelveVec.begin(); cShelve != fShelveVec.end(); cShelve++)
		{
			// Iterating over the Shelves

			for(uint8_t cBeId = 0; cBeId < (*cShelve)->getNBoard(); cBeId++)
			{
				BeBoard* board = (*cShelve)->getBoard(cBeId);
				// Loop over all BeBoards, Fes, Cbcs
			    for(uint8_t cFeId = 0; cFeId < board->getNFe(); cFeId++)
				{
					for(uint8_t cCbcId = 0; cCbcId < board->getModule(cFeId)->getNCbc(); cCbcId++)
					{
						for(uint8_t cGroupId = 0; cGroupId < 8; cGroupId++)
						{

							// Create a test Group to find the corresponding in the map
							TestGroup cTempTestGroup(cBeId, cFeId, cCbcId, cGroupId);

							// Find it!
							TestGroupMap::iterator cGroup = fTestGroupMap.find(cTempTestGroup);
							if (cGroup != fTestGroupMap.end())
							{
								// Iterator for the Channels in the TestGroup
								std::vector<Channel>::iterator cChannel = cGroup->second.begin();

								// Iterate through the channels and set Offset to 0x50
								// Vector of pairs for the write MultiReg
								std::vector< std::pair< std::string, uint8_t > > cRegVec;

								for( ; cChannel != cGroup->second.end(); cChannel++)
								{
									// Initialize the Histogram & fit for the current Vplus Settings
									cChannel->initializeHist((*cVplus), true);

									// Write the offset to the Cbcs
									TString cRegName = Form("Channel%03d",cChannel->fChannelId);
									uint8_t cRegValue = 0x50;

									std::pair<std::string, uint8_t> cRegPair = std::make_pair(cRegName.Data(), cRegValue);
									cRegVec.push_back(cRegPair);

									// Update the Channel Object
									cChannel->setOffset(0x50);
								}

								fCbcInterface->WriteCbcMultReg(board->getModule(cFeId)->getCbc(cCbcId),cRegVec);


								// Loop over a range of Vcth 
								// add sanity checks for the loop range here!!!

								for(uint8_t cVcth = cTargetVcth - 80; cVcth < cTargetVcth + 80; cVcth+=10)
								{
									uint32_t cN = 0;
									uint32_t cNthAcq = 0;

									while( cN <= cEventsperVcth )
									{

										Run(board, cNthAcq);

										// Run over a number of events
										// fBeBoardInterface.Start(board);
										// fBeBoardInterface.ReadData(board, cNthAcq, true );
										// fBeBoardInterface.Stop(board, cNthAcq );

										const Event *cEvent = fBeBoardInterface->GetNextEvent(board);

										// Loop over Events from this Acquisition
									    while( cEvent )
									    {

									        if( cN == cEventsperVcth )
									        {
									            break;
									        }

							                uint32_t cNHits = 0;
							                std::vector<bool> cDataBitVector = cEvent->DataBitVector(cGroup->first.fFeId, cGroup->first.fCbcId);

							                // Now loop over all channels in the TestGroup
							                // Iterator for the Channels in the TestGroup
				        					std::vector<Channel>::iterator cChannel = cGroup->second.begin();

				        					for( ; cChannel != cGroup->second.end(); cChannel++)
				        					{
							                    if(cDataBitVector[cChannel->fChannelId])
							                    {
							                    	cChannel->fillHist(cVcth);
							                        cNHits++;
							                    }
									        }

									        //cCanvas->Print(((boost::format("output/Histogram_Event_%d.pdf") %(cN)).str()).c_str());
									        cN++;

									        // 
									        if(cN <= cEventsperVcth ) cEvent = fBeBoardInterface->GetNextEvent(board);
									        else break;
										}
										cNthAcq++;
									}
								}  // done looping over Vcth, Scurves created

							    // The Scurves have been filled, now Fit them
								// Iterator for the Channels in the TestGroup
								// std::vector<Channel>::iterator cChannel = cGroup->second.begin();

								// clear cRegVec before re-using it
								cRegVec.clear();

								// Iterate through the channels and fit the Histogram
								for( ; cChannel != cGroup->second.end(); cChannel++)
								{
									cChannel->fitHist(cEventsperVcth, cHoleMode, *cVplus, fResultFile);

									// fill test Group TGraphErrors here as well!
									cGroup->first.FillVplusVcthGraph(*cVplus, cChannel->getPedestal(), cChannel->getNoise());

									cChannel->resetHist();

									// set offsets back to 0x00 or 0xFF
									TString cRegName = Form("Channel%03d",cChannel->fChannelId);
									uint8_t cRegValue;
									if (fHole) cRegValue = 0x00;
									else cRegValue = 0xFF;
									std::vector< std::pair< std::string, uint8_t > > cRegVec;
									std::pair<std::string, uint8_t> cRegPair = std::make_pair(cRegName.Data(), cRegValue);
									cRegVec.push_back(cRegPair);

									// Update the Channel Object
									cChannel->setOffset(0x00);
								}

								fCbcInterface->WriteCbcMultReg(board->getModule(cFeId)->getCbc(cCbcId),cRegVec);

								// Now Scurves & fits are stored in the Resultfile, the VplusvsVcth Graph is filled
							} 
							else std::cout << "This test group (BE" << cBeId << " FE" << cFeId << " CBC" << cCbcId << " Group" << cGroupId << ") was not found!" std::endl; 
						} // End of Test Group Loop
					} // End of Cbc Loop
				} // End of Fe Loop
			} // End of Be Loop
		} // End of the Shelve Loop
	} // End of Vplus Loop
}


uint8_t Calibration::FitVplusVcth(bool pDoDraw, uint8_t pTargetVcth){

	for(uint8_t cBeId = 0; cBeId < fShelf->getNBoard(); cBeId++)
	{
		// Loop over all BeBoards, Fes, Cbcs
	    for(uint8_t cFeId = 0; cFeId < board->getNFe(); cFeId++)
		{
			for(uint8_t cCbcId = 0; cCbcId < board->getModule(cFeId)->getNCbc(); cCbcId++)
			{
				TString multigraphname = Form("VplusVcth_Be%d_Fe%d_Cbc%d", cBeId, cFeId, cCbcId);
				TMultiGraph* cVplusVcthMultiGraph = new TMultiGraph(multigraphname,Form("Vplus vs. Vcth; Vcth; Vplus"));

				if (pDoDraw){
					TString canvasname = Form("Be%d_Fe%d_Cbc%d_VplusVcth", cBeId, cFeId, cCbcId);
					TCanvas* cVplusVcthCanvas = (TCanvas*) gROOT->FindObject(canvasname);
					if (cVplusVcthCanvas != NULL) delete cVplusVcthCanvas;
					else cVplusVcthCanvas = new TCanvas(canvasname, canvasname);
					cVplusVcthCanvas->cd();
				}

				for(uint8_t cGroupId = 0; cGroupId < 8; cGroupId++)
				{

					// Create a test Group to find the corresponding in the map
					TestGroup cTempTestGroup(cBeId, cFeId, cCbcId, cGroupId);

					// Find it!
					TestGroupMap::iterator cGroup = fTestGroupMap.find(cTempTestGroup);
					if (cGroup != fTestGroupMap.end())
					{
						*cGroup->first.fVplusVcthGraph->SetMarkerStyle(8);
						*cGroup->first.fVplusVcthGraph->SetMarkerColor(cGroupId+1);
						cVplusVcthMultiGraph->Add(*cGroup->first.fVplusVcthGraph);
					}
					else std::cout << "This test group (BE" << cBeId << " FE" << cFeId << " CBC" << cCbcId << " Group" << cGroupId << ") was not found!" std::endl; 
				}

				// All Points in the Multigraph; Fit it!
				TF1* cVplusVcthFit = cVplusVcthMultiGraph->Fit( "pol1", "QN", "", 1, hextoint(xFF));

				if (pDoDraw){
					cVplusVcthMultiGraph->Draw("AP");
					cVplusVcthFit->Draw("same");
				}
				// Add cVplusVcthFit->eval statement
				return cVplusVcthFit->Eval(pTargetVcth);
			}
		}
	}
}

uint8_t Calibration::hextoint(har*pHexValue){
	return strtol(pHexValue, NULL, 16);
}
	



