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

void Calibration::ConstructTestGroup(uint8_t pBeId, uint8_t pFeId, uint8_t pCbcId)
{
	for(uint8_t lgroup=0; lgroup<8; lgroup++)
	{
		TestGroup fTestgroup(pBeId,pFeId,pCbcId,lgroup);
				
		std::cout<<"	Group: "<<int(lgroup)<<"created!"<<std::endl;

		std::vector<Channel> cChannelVect;
		TestGroupGraph cTestGroupGraph(pBeId, pFeId, pCbcId, lgroup);

		for(uint8_t lchannel=0; lchannel<16; lchannel++)
		{
					
			if (lchannel*16+lgroup*2 < 254)
			{
				Channel cChannel(pBeId,pFeId,pCbcId, lchannel*16+lgroup*2 );
				std::cout<<"		Channel: "<<int(lchannel*16+lgroup*2)<<"created!"<<std::endl;
				cChannelVect.push_back(cChannel);
			}

			if ((lchannel*16+lgroup*2) +1 < 254)
			{
				Channel cChannel(pBeId,pFeId,pCbcId, (lchannel*16+lgroup*2) +1 );
				std::cout<<"		Channel: "<<int( (lchannel*16+lgroup*2) +1)<<"created!"<<std::endl;
				cChannelVect.push_back(cChannel);
			}

		}

		fTestGroupMap[fTestgroup]=cChannelVect;
		fTestGroupGraphMap[fTestgroup]=cTestGroupGraph;
	}

}	


void Calibration::VplusScan(){

	// for 4 different values of Vplus, loop over all Test Groups on all Cbc's simultaniously:
	// 1. enable test groups on all Cbc's on 1 FE
	// 2. Loop over Vcth Range and set Vcth on all Cbc's
	// 3. Run and Take Data for all Cbc's on 1 FE
	// 4. Get Event and again loop over all Cbc's on 1 FE and analyze the data

	fVplusValues.push_back(0x40);
	fVplusValues.push_back(0x50);
	fVplusValues.push_back(0x60);
	fVplusValues.push_back(0x70);

	uint32_t cEventsperVcth = fSettingsMap.find("Nevents")->second;
	bool cHoleMode = fSettingsMap.find("HoleMode")->second;
	uint8_t cTargetVcth = fSettingsMap.find("TargetVcth")->second;

	// Loop over 4 values of Vplus for the Vplus vs. Pedestal (Vcth) graph
	// for(std::vector<uint8_t>::iterator cVplus = fVplusValues.begin(); cVplus != fVplusValues.end(); cVplus++)
	for(uint cVplusindex = 0; cVplusindex < fVplusValues.size(); cVplusindex++)
	{
		uint8_t cVplus = fVplusValues.at(cVplusindex);

		for(std::vector<Shelve*>::iterator cShelve = fShelveVec.begin(); cShelve != fShelveVec.end(); cShelve++)
		{
			// Iterating over the Shelves

			for(uint8_t cBeId = 0; cBeId < (*cShelve)->getNBoard(); cBeId++)
			{
				BeBoard* board = (*cShelve)->getBoard(cBeId);
				// Loop over all BeBoards, Fes, Cbcs
			    for(uint8_t cFeId = 0; cFeId < board->getNFe(); cFeId++)
				{
					for(uint8_t cGroupId = 0; cGroupId < 8; cGroupId++)
					{
						for(uint8_t cCbcId = 0; cCbcId < board->getModule(cFeId)->getNCbc(); cCbcId++)
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
									cChannel->initializeHist((cVplus), true);

									// Write the offset to the Cbcs
									TString cRegName = Form("Channel%03d",cChannel->fChannelId);
									uint8_t cRegValue = 0x50;

									std::pair<std::string, uint8_t> cRegPair = std::make_pair(cRegName.Data(), cRegValue);
									cRegVec.push_back(cRegPair);

									// Update the Channel Object
									cChannel->setOffset(0x50);
								}

								fCbcInterface->WriteCbcMultReg(board->getModule(cFeId)->getCbc(cCbcId),cRegVec);
							} //if statement that group is found
						} // Cbc Loop
					} // Test Group Loop
				} // Fe Loop

				// Now the Test groups offset is set accordingly


				// Loop over a range of Vcth and acquire data
				// add sanity checks for the loop range here!!!

				for(uint8_t cVcth = cTargetVcth - 80; cVcth < cTargetVcth + 80; cVcth+=10)
				{
					// Set current Vcth value on all Cbc's of the current board
				    for(uint8_t cFeId = 0; cFeId < board->getNFe(); cFeId++)
					{
							for(uint8_t cCbcId = 0; cCbcId < board->getModule(cFeId)->getNCbc(); cCbcId++)
							{
								fCbcInterface->WriteCbcReg(board->getModule(cFeId)->getCbc(cCbcId),"VCth",cTargetVcth);
							}
					}
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

					        // Here Loop over all FE's Cbc's, Test Groups & Channels to fill Histograms
					            for(uint8_t cFeId = 0; cFeId < board->getNFe(); cFeId++)
					        	{
					        		for(uint8_t cGroupId = 0; cGroupId < 8; cGroupId++)
					        		{
					        			for(uint8_t cCbcId = 0; cCbcId < board->getModule(cFeId)->getNCbc(); cCbcId++)
					        			{
					        				// Create a test Group to find the corresponding in the map
					        				TestGroup cTempTestGroup(cBeId, cFeId, cCbcId, cGroupId);

					        				// Find it!
					        				TestGroupMap::iterator cGroup = fTestGroupMap.find(cTempTestGroup);
					        				if (cGroup != fTestGroupMap.end())
					        				{
								                uint32_t cNHits = 0;
								                std::vector<bool> cDataBitVector = cEvent->DataBitVector(cGroup->first.fFeId, cGroup->first.fCbcId);

								                // Now loop over all channels in the TestGroup
					        					std::vector<Channel>::iterator cChannel = cGroup->second.begin();

					        					for( ; cChannel != cGroup->second.end(); cChannel++)
					        					{
								                    if(cDataBitVector[cChannel->fChannelId])
								                    {
								                    	cChannel->fillHist(cVcth);
								                        cNHits++;
								                    }
										        } // Channel Loop
										    } // If Statement
										    else std::cout << "Test Group not found, something is wrong!" << std::endl;
										} // Cbc Loop
									} // Group Loop
								} // Fe Loop

					        cN++;

					        // 
					        if(cN <= cEventsperVcth ) cEvent = fBeBoardInterface->GetNextEvent(board);
					        else break;
						}
						cNthAcq++;
					} // End of Analyze Events of last Acquistion loop
				}  // done looping over Vcth, Scurves created

				// Now Fit the hists & fill the VplusVcth Graph & set the TestGroup Offset back to 0x00 or 0xFF

				// Here Loop over all FE's Cbc's, Test Groups & Channels to fill Histograms
			    for(uint8_t cFeId = 0; cFeId < board->getNFe(); cFeId++)
				{
					for(uint8_t cGroupId = 0; cGroupId < 8; cGroupId++)
					{
						for(uint8_t cCbcId = 0; cCbcId < board->getModule(cFeId)->getNCbc(); cCbcId++)
						{
							// Create a test Group to find the corresponding in the map
							TestGroup cTempTestGroup(cBeId, cFeId, cCbcId, cGroupId);

							// Find it!
							TestGroupMap::iterator cGroup = fTestGroupMap.find(cTempTestGroup);
							if (cGroup != fTestGroupMap.end())
							{
								// Iterator for the Channels in the TestGroup
								std::vector<Channel>::iterator cChannel = cGroup->second.begin();
								// Vector of pairs for the write MultiReg
								std::vector< std::pair< std::string, uint8_t > > cRegVec;
							
								// Iterate through the channels and fit the Histogram
								for( ; cChannel != cGroup->second.end(); cChannel++)
								{
									cChannel->fitHist(cEventsperVcth, cHoleMode, cVplus, fResultFile);

									// fill test Group TGraphErrors here 
									TestGroupGraphMap::iterator cGraphIt = fTestGroupGraphMap.find(cTempTestGroup);
									if(cGraphIt != fTestGroupGraphMap.end())
										cGraphIt->second.FillVplusVcthGraph(cVplus, cChannel->getPedestal(), cChannel->getNoise());
									else std::cout << "The Graph for this test Group could not be found! There is a problem!" << std::endl;
									

									// Reset the Histogram for the next value of Vplus
									cChannel->resetHist();

									// set offsets back to 0x00 or 0xFF
									TString cRegName = Form("Channel%03d",cChannel->fChannelId);
									uint8_t cRegValue;
									if (cHoleMode) cRegValue = 0x00;
									else cRegValue = 0xFF;
									std::pair<std::string, uint8_t> cRegPair = std::make_pair(cRegName.Data(), cRegValue);
									cRegVec.push_back(cRegPair);

									// Update the Channel Object
									cChannel->setOffset(0x00);
								}

								fCbcInterface->WriteCbcMultReg(board->getModule(cFeId)->getCbc(cCbcId),cRegVec);
							}
							else std::cout << "This test group (BE" << cBeId << " FE" << cFeId << " CBC" << cCbcId << " Group" << cGroupId << ") was not found!" << std::endl; 
						} // End of Cbc Loop
					} // End of Test Group Loop
				} // End of Fe Loop

			} // End of Be Loop
		} // End of the Shelve Loop
	} // End of Vplus Loop
}


void Calibration::FitVplusVcth(bool pDoDraw, uint8_t pTargetVcth){

	for(std::vector<Shelve*>::iterator cShelve = fShelveVec.begin(); cShelve != fShelveVec.end(); cShelve++)
	{
		// Iterating over the Shelves
		for(uint8_t cBeId = 0; cBeId < (*cShelve)->getNBoard(); cBeId++)
		{
			// Loop over all BeBoards, Fes, Cbcs
			BeBoard* board = (*cShelve)->getBoard(cBeId);

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
						TestGroupGraphMap::iterator cGroup = fTestGroupGraphMap.find(cTempTestGroup);
						if (cGroup != fTestGroupGraphMap.end())
						{
							cGroup->second.fVplusVcthGraph->SetMarkerStyle(8);
							cGroup->second.fVplusVcthGraph->SetMarkerColor(cGroupId+1);
							cVplusVcthMultiGraph->Add(cGroup->second.fVplusVcthGraph);
						}
						else std::cout << "This test group (BE" << cBeId << " FE" << cFeId << " CBC" << cCbcId << " Group" << cGroupId << ") was not found!" << std::endl; 
					}

					// All Points in the Multigraph; Fit it per Cbc, draw it Per Cbc
					TFitResultPointer* cVplusVcthFit = cVplusVcthMultiGraph->Fit( "pol1", "Q", "", 1, 0xFF);

					if (pDoDraw){
						cVplusVcthMultiGraph->Draw("AP");
						cVplusVcthFit->Draw("same");
					}
					// Get the right Vplus setting & write to the Cbc
					uint8_t cVplusResult = (uint8_t) ((int) cVplusVcthFit->Eval(pTargetVcth));

					fCbcInterface->WriteCbcReg(board->getModule(cFeId)->getCbc(cCbcId),"Vplus",cVplusResult);
					std::cout << "Vplus Setting for Be " << cBeId << " Fe " << cFeId << " Cbc " << cCbcId << " : " << std::hex << cVplusResult << std::endl;

				} // End of Cbc Loop
			} // End of Fe Loop
		} // End of Be Loop
	} // End of Shelve Loop
}

	



