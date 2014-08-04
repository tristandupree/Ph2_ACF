/*!
*
* \file GlibFWInterface.cc
* \brief GlibFWInterface init/config of the Glib
* \author Lorenzo BIDEGAIN, Nicolas Pierre
* \date 28/07/14
*
* Support : mail to : lorenzo.bidegain@cern.ch, nicolas.pierre@cern.ch
*
*/

#include <uhal/uhal.hpp>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <time.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TStyle.h>
#include "GlibFWInterface.h"

#define DEV_FLAG         0

namespace Ph2_HwInterface
{

	GlibFWInterface::GlibFWInterface(const char *puHalConfigFileName):BeBoardFWInterface(puHalConfigFileName){
	}


	void GlibFWInterface::ConfigureGlib(Glib& pGlib)
    	{

        	//We may here switch in the future with the StackReg method of the RegManager
        	//when the timeout thing will be implemented in a transparent and pretty way

        	std::vector< std::pair<std::string,uint32_t> > cVecReg;
        	std::pair<std::string,uint32_t> cPairReg;

        	ChooseBoard(pGlib.getBeId());

        	boost::posix_time::milliseconds cPause(200);

        	//Primary Configuration
        	cPairReg.first = SRAM1_END_READOUT; cPairReg.second = 0;
		cVecReg.push_back(cPairReg);
        	cPairReg.first = SRAM2_END_READOUT; cPairReg.second = 0;
        	cVecReg.push_back(cPairReg);
        	cPairReg.first = SRAM1_USR_LOGIC; cPairReg.second = 1;
        	cVecReg.push_back(cPairReg);
        	cPairReg.first = SRAM2_USR_LOGIC; cPairReg.second = 1;
        	cVecReg.push_back(cPairReg);

        	WriteStackReg(cVecReg);

        	cVecReg.clear();

		boost::this_thread::sleep(cPause);


        	/*
       		GlibRegMap : map<std::string,uint8_t> created from Glib class

        	Mandatory to go through a created cGlibRegMap.
        	If you want to put directly pGlib.getGlibRegMap(), you'll end up with
        	a seg fault error, as it is not putting all the map in mem but only
        	begin() and end().
        	*/

        	BeBoardRegMap cGlibRegMap = pGlib.getBeBoardRegMap();
		for(BeBoardRegMap::iterator cIt = cGlibRegMap.begin(); cIt != cGlibRegMap.end(); ++cIt )
        	{
        		cPairReg.first = cIt->first; cPairReg.second = cIt->second;
            		cVecReg.push_back(cPairReg);
		}

        	WriteStackReg(cVecReg);

        	cVecReg.clear();

        	cPairReg.first = SPURIOUS_FRAME; cPairReg.second = 0;
        	cVecReg.push_back(cPairReg);
        	cPairReg.first = FORCE_BG0_START; cPairReg.second = 0;
        	cVecReg.push_back(cPairReg);
        	cPairReg.first = CBC_TRIGGER_1SHOT; cPairReg.second = 0;
        	cVecReg.push_back(cPairReg);
        	cPairReg.first = BREAK_TRIGGER; cPairReg.second = 1;
        	cVecReg.push_back(cPairReg);

        	WriteStackReg(cVecReg);

        	cVecReg.clear();


        	cPairReg.first = PC_CONFIG_OK; cPairReg.second = 0;
        	cVecReg.push_back(cPairReg);
        	cPairReg.first = SRAM1_END_READOUT; cPairReg.second = 0;
        	cVecReg.push_back(cPairReg);
        	cPairReg.first = SRAM2_END_READOUT; cPairReg.second = 0;
        	cVecReg.push_back(cPairReg);
        	cPairReg.first = SRAM1_USR_LOGIC; cPairReg.second = 1;
        	cVecReg.push_back(cPairReg);
        	cPairReg.first = SRAM2_USR_LOGIC; cPairReg.second = 1;
        	cVecReg.push_back(cPairReg);

        	WriteStackReg(cVecReg);

        	cVecReg.clear();

		boost::this_thread::sleep(cPause);


        	cPairReg.first = SPURIOUS_FRAME; cPairReg.second = 0;
        	cVecReg.push_back(cPairReg);
        	cPairReg.first = FORCE_BG0_START; cPairReg.second = 0;
        	cVecReg.push_back(cPairReg);
        	cPairReg.first = CBC_TRIGGER_1SHOT; cPairReg.second = 0;
        	cVecReg.push_back(cPairReg);
        	cPairReg.first = BREAK_TRIGGER; cPairReg.second = 1;
        	cVecReg.push_back(cPairReg);

        	WriteStackReg(cVecReg);

        	cVecReg.clear();

		boost::this_thread::sleep( cPause*3 );

    	}

	void GlibFWInterface::SelectFEId()
	{
		if(uint32_t(ReadReg(HYBRID_TYPE)) == 8)
		{
			fCbcStubLat  = (uint32_t(ReadReg(FMC1_PRESENT)) ? CBC_STUB_LATENCY_FE1 : CBC_STUB_LATENCY_FE2);
			fCbcI2CCmdAck =  (uint32_t(ReadReg(FMC1_PRESENT)) ? CBC_I2C_CMD_ACK_FE1 : CBC_I2C_CMD_ACK_FE2);
			fCbcI2CCmdRq = (uint32_t(ReadReg(FMC1_PRESENT)) ? CBC_I2C_CMD_RQ_FE1 : CBC_I2C_CMD_RQ_FE2);
			fCbcHardReset = (uint32_t(ReadReg(FMC1_PRESENT)) ? CBC_HARD_RESET_FE1 : CBC_HARD_RESET_FE2);
			fCbcFastReset = (uint32_t(ReadReg(FMC1_PRESENT)) ? CBC_FAST_RESET_FE1 : CBC_FAST_RESET_FE2);
		}
        	else
        	{
            	fCbcStubLat  = CBC_STUB_LATENCY;
            	fCbcI2CCmdAck =  CBC_I2C_CMD_ACK;
            	fCbcI2CCmdRq = CBC_I2C_CMD_RQ;
            	fCbcHardReset = CBC_HARD_RESET;
            	fCbcFastReset = CBC_FAST_RESET;
        	}
    	}

	void GlibFWInterface::Start(Glib& pGlib)
	{

		#ifdef __CBCDAQ_DEV__
		long mtime = getTimeTook( fStartVeto, 1 );
		std::cout << "Time took for the trigger veto to trigger enable: " << std::dec << mtime << " ms." << std::endl;
		#endif

		std::vector< std::pair<std::string,uint32_t> > cVecReg;
		std::pair<std::string,uint32_t> cPairReg;

        	ChooseBoard(pGlib.getBeId());

        	//Starting the DAQ

        	cPairReg.first = BREAK_TRIGGER; cPairReg.second = 0;
        	cVecReg.push_back(cPairReg);
        	cPairReg.first = PC_CONFIG_OK; cPairReg.second = 1;
        	cVecReg.push_back(cPairReg);
        	cPairReg.first = FORCE_BG0_START; cPairReg.second = 1;
        	cVecReg.push_back(cPairReg);

        	WriteStackReg(cVecReg);

        	cVecReg.clear();

    }

	void GlibFWInterface::Stop(Glib& pGlib, uint32_t pNthAcq )
    	{

        	std::vector< std::pair<std::string,uint32_t> > cVecReg;
        	std::pair<std::string,uint32_t> cPairReg;

		uhal::ValWord<uint32_t> cVal;

        	ChooseBoard(pGlib.getBeId());

        	//Select SRAM
        	SelectSRAM( pNthAcq );

        	//Stop the DAQ
        	cPairReg.first = BREAK_TRIGGER; cPairReg.second = 1;
        	cVecReg.push_back(cPairReg);
        	cPairReg.first = PC_CONFIG_OK; cPairReg.second = 0;
        	cVecReg.push_back(cPairReg);
        	cPairReg.first = FORCE_BG0_START; cPairReg.second = 0;
        	cVecReg.push_back(cPairReg);

        	WriteStackReg(cVecReg);
        	cVecReg.clear();

        	boost::posix_time::milliseconds cWait(100);

        	//Wait for the selected SRAM to be full then empty it
		do
        	{
			cVal = ReadReg(fStrFull);

            		if (cVal==1)
                	boost::this_thread::sleep(cWait);

        	} while (cVal==1);

        	WriteReg(fStrReadout,0);
        	fNTotalAcq++;
    	}

    	void GlibFWInterface::Pause(Glib& pGlib)
    	{

        	ChooseBoard(pGlib.getBeId());

        	WriteReg(BREAK_TRIGGER,1);

		#ifdef __CBCDAQ_DEV__
        	std::cout << "Pause engaged" << std::endl;
		#endif
    	}

	void GlibFWInterface::Unpause(Glib& pGlib)
    	{
        	ChooseBoard(pGlib.getBeId());

        	WriteReg(BREAK_TRIGGER,0);

		#ifdef __CBCDAQ_DEV__
        	std::cout << "Pause disengaged" << std::endl;
		#endif
    	}

	void GlibFWInterface::ReadData(Glib& pGlib, unsigned int pNthAcq, bool pBreakTrigger )
    	{

		#ifdef __CBCDAQ_DEV__
        	std::cout << "ReadDataInSRAM" << std::endl;

		struct timeval cStartReadDataInSRAM, cStartBlockRead;
		struct timeval start;
		long mtime;

		gettimeofday( &cStartReadDataInSRAM, 0 );
		gettimeofday(&start, 0);
		#endif

        	ChooseBoard(pGlib.getBeId());

		//Readout settings
		boost::posix_time::milliseconds cWait(1);

		uhal::ValWord<uint32_t> cVal;
		uint32_t cNPackets= EVENT_NUMBER+1;
		uint32_t cBlockSize = cNPackets * PACKET_SIZE;

		//Wait for start acknowledge
		do
        	{
			cVal=ReadReg(CMD_START_VALID);

			if ( cVal==0 )
				boost::this_thread::sleep(cWait);

		} while ( cVal==0 );

		#ifdef __CBCDAQ_DEV__
		mtime = getTimeTook( start, 1 );
		std::cout << "GlibController::ReadData()  Time took for the CMD_START_VALID flag to be set: " << 			std::dec << mtime << " ms." << std::endl;
		#endif

		//FIFO goes to write_data state
		//Select SRAM
		SelectSRAM( pNthAcq );

		#ifdef __CBCDAQ_DEV__
		gettimeofday(&start, 0);
		#endif

		//Wait for the SRAM full condition.
		cVal = ReadReg(fStrFull);

        	do
        	{
			boost::this_thread::sleep( cWait );

			cVal = ReadReg(fStrFull);

		} while (cVal==0);

		#ifdef __CBCDAQ_DEV__
		mtime = getTimeTook( start, 1 );
		std::cout << "Time took for the data to be ready : " << std::dec << mtime << " ms." << std::endl;
		#endif

		//break trigger
		if( pBreakTrigger )
        	{
			WriteReg(BREAK_TRIGGER,1);
		}

        	// JRF end
		#ifdef __CBCDAQ_DEV__
		gettimeofday( &fStartVeto, 0 );
		#endif

		//Set read mode to SRAM
		WriteReg(fStrSramUserLogic,0);

		#ifdef __CBCDAQ_DEV__
	    	gettimeofday( &cStartBlockRead, 0 );
		#endif

		//Read SRAM
		uhal::ValVector<uint32_t> cData = ReadBlockReg(fStrSram,cBlockSize);

		#ifdef __CBCDAQ_DEV__
		mtime = getTimeTook( cStartBlockRead, 1 );
		std::cout << "Time took for block read: " << std::dec << mtime << " ms." << std::endl;
		#endif

		WriteReg(fStrSramUserLogic,1);
		WriteReg(fStrReadout,1);

		#ifdef __CBCDAQ_DEV__
		gettimeofday(&start, 0);
		#endif

		//Wait for the non SRAM full condition starts,
		do
        	{
			cVal = ReadReg(fStrFull);

			if (cVal==1)
				boost::this_thread::sleep(cWait);

		} while (cVal==1);

		//Wait for the non SRAM full condition ends.
		#ifdef __CBCDAQ_DEV__
		mtime = getTimeTook( start, 0 );
		std::cout << "Time took to the full flag to be 0 : " << std::dec << mtime << " us." << std::endl;
		#endif

		WriteReg(fStrReadout,0);

		#ifdef __CBCDAQ_DEV__
		mtime = getTimeTook( cStartReadDataInSRAM, 1 );
		std::cout << "Time took for ReadDataInSRAM: " << std::dec << mtime << " ms." << std::endl;
		#endif

        	//One data for one event --> Enhanced later
		fData->Set(&cData);

	}

	void GlibFWInterface::Run(Glib& pGlib)
	{
		fStop = false;

		std::ofstream cfile( "output/TestData.dat", std::ios::out | std::ios::trunc );

		uint32_t cNthAcq = 0;
		uint32_t cNevents = 10;
		uint32_t cN = 0;

        	TCanvas *cCanvas = new TCanvas("Data Acq", "Different hits counters", 600, 400);
        	TCanvas *cCanvasMean = new TCanvas("Data Acq Mean", "Different hits counters", 600, 400);
        	TH1F *cHist = NULL;
        	TH1F *cHistMean = new TH1F("Histo_Hits Mean", "Hit Counter", uint32_t(pGlib.getModule(0)->getNCbc()), 0., uint32_t(pGlib.getModule(0)->getNCbc()));
        	gStyle->SetOptStat(kFALSE);

        	usleep( 100 );

        	fData->Initialise( EVENT_NUMBER, pGlib );

		while(!fStop)
        	{

            		Start(pGlib);
            		ReadData(pGlib, cNthAcq, true );
            		Stop(pGlib, cNthAcq );

            		bool cFillDataStream( false );

            		const Event *cEvent = fData->GetNextEvent();

            		while( cEvent )
            		{

                		if( cNevents != 0 && cN == cNevents )
                		{
                    			fStop = true;
                    			break;
                		}

                		cCanvas->Divide(uint32_t(pGlib.getNFe()),1);

                		for(uint8_t i=0; i<pGlib.getNFe(); i++)
                		{
                    			cfile << "Event N°" << cN+1 << std::endl;
                    			cfile << "FE " << uint32_t(i) << " :" << std::endl;

                    			if(cHist != NULL)
                        		delete cHist;

                    			cHist = new TH1F("Histo_Hits", "Hit Counter", uint32_t(pGlib.getModule(i)->getNCbc()), 0., uint32_t(pGlib.getModule(i)->getNCbc()));

                    			cCanvas->cd(uint32_t(i));

                    			for(uint8_t j=0; j<pGlib.getModule(i)->getNCbc(); j++)
                    			{
                        			uint32_t cNHits = 0;

                        			std::vector<bool> cDataBitVector = cEvent->DataBitVector(i,j);

                        			cfile << "CBC " << uint32_t(j) << " : " << cEvent->DataBitString(i,j) << std::endl;

                        			for(uint32_t k=0; k<cDataBitVector.size(); k++)
                        			{
                            				if(cDataBitVector[k])
                            				{
                                				cNHits++;
                            				}
                        			}

                        			cHist->Fill(uint32_t(j),cNHits);
                        			cHistMean->Fill(uint32_t(j),cNHits/cNevents);
                    			}

                   			cHist->Draw();
                    			cCanvas->Update();

                    			cfile << "\n";
				}

                		cCanvas->Print(((boost::format("output/Histogram_Event_%d.pdf") %(cN)).str()).c_str());

                		cEvent = fData->GetNextEvent();

                		cFillDataStream = false;
                		cN++;
            		}

			if( cN == cNevents )
            		{
                		fStop = true;
                		break;
            		}

        	}

		cHistMean->Draw();
        	cCanvasMean->Update();
        	cCanvas->Print("output/Histogram_Mean.pdf");

        	delete cHist;

        	cfile.close();
	}

	void GlibFWInterface::SelectSRAM(uint32_t pNthAcq)
    	{
        	fStrSram  = ((pNthAcq%2+1)==1 ? SRAM1 : SRAM2);
		fStrOtherSram = (pNthAcq ? SRAM1 : SRAM2);
        	fStrSramUserLogic =  ((pNthAcq%2+1)==1 ? SRAM1_USR_LOGIC : SRAM2_USR_LOGIC);
        	fStrFull = ((pNthAcq%2+1)==1 ? SRAM1_FULL : SRAM2_FULL);
        	fStrReadout= ((pNthAcq%2+1)==1 ? SRAM1_END_READOUT : SRAM2_END_READOUT);
    	}

	//Methods for Cbc's:

	bool GlibFWInterface::I2cCmdAckWait( uint32_t pAckVal, uint8_t pNcount )
	{
        	unsigned int cWait(100);

        	if( pAckVal )
        	{
			cWait = pNcount * 500;
		}

		usleep( cWait );

		uhal::ValWord<uint32_t> cVal;
		uint32_t cLoop=0;

        	do
        	{
			cVal=ReadReg(CBC_I2C_CMD_ACK);

			if (cVal!=pAckVal)
            		{
				std::cout << "Waiting for the I2c command acknowledge to be " << pAckVal << " for " << uint32_t(pNcount) << " registers." << std::endl;
				usleep( cWait );
			}

		} while (cVal!= pAckVal && ++cLoop<MAX_NB_LOOP);

		if (cLoop>=MAX_NB_LOOP)
        		{
			std::cout<<"Warning: time out in I2C acknowledge loop (" << pAckVal << ")" << std::endl;
			return false;
			}

		return true;
	}

	void GlibFWInterface::SendBlockCbcI2cRequest(std::vector<uint32_t>& pVecReq, bool pWrite)
	{

		WriteReg(fStrSramUserLogic,1);

		pVecReq.push_back(0xFFFFFFFF);

		WriteReg(fStrSramUserLogic,0);

		WriteBlockReg(fStrSram,pVecReq);
        	WriteReg(fStrOtherSram,0xFFFFFFFF);

		WriteReg(fStrSramUserLogic,1);

        	WriteReg(CBC_HARD_RESET,0);

        	//r/w request
		WriteReg(CBC_I2C_CMD_RQ,pWrite ? 3: 1);

		pVecReq.pop_back();

        	if( I2cCmdAckWait( (uint32_t)1, pVecReq.size() ) ==0 )
        	{
            		throw Exception( Form( "%s: I2cCmdAckWait %d failed.", "CbcInterface", 1 ) );
        	}

        	WriteReg(CBC_I2C_CMD_RQ,0);

        	if( I2cCmdAckWait( (uint32_t)0, pVecReq.size() ) ==0 )
        	{
            		throw Exception( Form( "%s: I2cCmdAckWait %d failed.", "CbcInterface", 0 ) );
        	}

	}

	void GlibFWInterface::ReadI2cBlockValuesInSRAM(std::vector<uint32_t> &pVecReq )
    	{

        	WriteReg(fStrSramUserLogic,0);

		uhal::ValVector<uint32_t> cData = ReadBlockReg(fStrSram,pVecReq.size());

		WriteReg(fStrSramUserLogic,1);
		WriteReg(CBC_I2C_CMD_RQ,0);

		std::vector<uint32_t>::iterator it = pVecReq.begin();
		uhal::ValVector< uint32_t >::const_iterator itValue = cData.begin();

       		while( it != pVecReq.end() )
        	{
			*it = *itValue;
			it++; itValue++;
		}
    	}


	void GlibFWInterface::EnableI2c( Cbc* pCbc, bool pEnable )
    	{
		ChooseBoard(pCbc->fBeId);

		uint32_t cValue = I2C_CTRL_ENABLE;

       		if( !pEnable )
            	cValue = I2C_CTRL_DISABLE;

		WriteReg(I2C_SETTINGS,cValue);

        	if( pEnable )
           	usleep(100000);
	}

	void GlibFWInterface::WriteCbcBlockReg( Cbc* pCbc, std::vector<uint32_t>& pVecReq )
	{
		#ifdef __CBCDAQ_DEV__
		static long min(0), sec(0);
		struct timeval start0, end;
		long seconds(0), useconds(0);

		if(  DEV_FLAG )
			{
			gettimeofday(&start0, 0);
			}
		#endif

		EnableI2c(pCbc,1);
        	SelectSRAM(uint32_t(pCbc->getCbcId()));

		try
		{
			SendBlockCbcI2cRequest( pVecReq, true );
		}

		catch( Exception &except )
		{
			throw except;
		}

		#ifdef __CBCDAQ_DEV__
		if( DEV_FLAG ){
			gettimeofday( &end, 0 );
			seconds = end.tv_sec - start0.tv_sec;
			useconds = end.tv_usec - start0.tv_usec;
			min += ( seconds + useconds / 1000000 ) /60;
			sec += ( seconds + useconds / 1000000 ) %60;
			std::cout << "Time took for Cbc register write so far = " << min << " min " << sec << " sec." << std::endl;

		}
		#endif

		EnableI2c(pCbc,0);
	}

	void GlibFWInterface::ReadCbcBlockReg( Cbc* pCbc, std::vector<uint32_t>& pVecReq )
	{

		#ifdef __CBCDAQ_DEV__
		static long min(0), sec(0);
		struct timeval start0, end;
		long seconds(0), useconds(0);

		if(  DEV_FLAG )
			{
			gettimeofday(&start0, 0);
			}
		#endif

		EnableI2c(pCbc,1);
        	SelectSRAM(uint32_t(pCbc->getCbcId()));

		try
		{
			SendBlockCbcI2cRequest(pVecReq, false);
		}

		catch( Exception &e )
		{
			throw e;
		}

		#ifdef __CBCDAQ_DEV__
		if(DEV_FLAG)
        	{
			gettimeofday( &end, 0 );
			seconds = end.tv_sec - start0.tv_sec;
			useconds = end.tv_usec - start0.tv_usec;
			min += ( seconds + useconds / 1000000 ) /60;
			sec += ( seconds + useconds / 1000000 ) %60;
			std::cout << "Time took for Cbc register read so far = " << min << " min " << sec << " sec." << std::endl;
		}
		#endif

		ReadI2cBlockValuesInSRAM( pVecReq );

		EnableI2c(pCbc,0);

	}
}
