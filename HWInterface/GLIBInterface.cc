/*

    FileName :                    GlibInterface.cc
    Content :                     GlibInterface class, init/config of the Glib
    Programmer :                  Nicolas PIERRE
    Version :                     0.3
    Date of creation :            07/06/14
    Support :                     mail to : nicolas.pierre@icloud.com

*/

#include <uhal/uhal.hpp>
#include <boost/format.hpp>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <time.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TStyle.h>
#include "GLIBInterface.h"
#include "Utilities.h"
#include "../HWDescription/Definition.h"

#define DEV_FLAG         0


namespace Ph2_HwInterface
{

    //Constructor, makes the board map
    GlibInterface::GlibInterface(const char *puHalConfigFileName):
        RegManager(puHalConfigFileName),
        fNTotalAcq(0),
        fDataFile(0),
		//fNegativeLogicCBC(true),
		fStop(false)
    {
        fData = new Data();
    }


    GlibInterface::~GlibInterface()
    {
        delete fData;
    }


    void GlibInterface::getBoardInfo(Glib& pGlib)
    {
        std::cout << "FMC1 present : " << uint32_t(ReadReg(FMC1_PRESENT)) << std::endl;
        std::cout << "FMC2 present : " << uint32_t(ReadReg(FMC2_PRESENT)) << std::endl;
        std::cout << "FW version : " << uint32_t(ReadReg(FW_VERSION_MAJOR)) << "." << uint32_t(ReadReg(FW_VERSION_MINOR)) << "." << uint32_t(ReadReg(FW_VERSION_BUILD)) << std::endl;

        uhal::ValWord<uint32_t> cBoardType = ReadReg(BOARD_TYPE);

        uint32_t cMask(0x00000000);
        unsigned int i(0);
        char cChar;

        std::cout << "BoardType : ";

        for( i=24; i < 32; i++ ) cMask |= ( (uint32_t) 1 << i );
        cChar = ( ( cBoardType & cMask ) >> 24 );

        std::cout << cChar;

        for( cMask=0, i=16; i < 24; i++ ) cMask |= (uint32_t) 1 << i;
        cChar = (( cBoardType & cMask ) >> 16);

        std::cout << cChar;

        for( cMask=0, i=8; i < 16; i++ ) cMask |= (uint32_t) 1 << i;
        cChar = (( cBoardType & cMask ) >> 8);

        std::cout << cChar;

        for( cMask=0, i=0; i < 8; i++ ) cMask |= (uint32_t) 1 << i;
        cChar = (cBoardType & cMask);

        std::cout << cChar << std::endl;

        std::cout << "FMC User Board ID : " << uint32_t(ReadReg(FMC_USER_BOARD_ID)) << std::endl;
        std::cout << "FMC User System ID : " << uint32_t(ReadReg(FMC_USER_SYS_ID)) << std::endl;
        std::cout << "FMC User Version : " << uint32_t(ReadReg(FMC_USER_VERSION)) << std::endl;

    }


    void GlibInterface::SelectFEId(Glib& pGlib)
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


    void GlibInterface::ConfigureGlib(Glib& pGlib)
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


    void GlibInterface::Start(Glib& pGlib)
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


    void GlibInterface::Stop(Glib& pGlib, uint32_t pNthAcq )
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


    void GlibInterface::Pause(Glib& pGlib)
    {

        ChooseBoard(pGlib.getBeId());

        WriteReg(BREAK_TRIGGER,1);

#ifdef __CBCDAQ_DEV__
        std::cout << "Pause engaged" << std::endl;
#endif
    }


    void GlibInterface::Unpause(Glib& pGlib)
    {
        ChooseBoard(pGlib.getBeId());

        WriteReg(BREAK_TRIGGER,0);

#ifdef __CBCDAQ_DEV__
        std::cout << "Pause disengaged" << std::endl;
#endif
    }


    void GlibInterface::ReadData(Glib& pGlib, unsigned int pNthAcq, bool pBreakTrigger )
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
		std::cout << "GlibController::ReadData()  Time took for the CMD_START_VALID flag to be set: " << std::dec << mtime << " ms." << std::endl;
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


    void GlibInterface::SelectSRAM(uint32_t pNthAcq)
    {
        fStrSram  = ((pNthAcq%2+1)==1 ? SRAM1 : SRAM2);
        fStrSramUserLogic =  ((pNthAcq%2+1)==1 ? SRAM1_USR_LOGIC : SRAM2_USR_LOGIC);
        fStrFull = ((pNthAcq%2+1)==1 ? SRAM1_FULL : SRAM2_FULL);
        fStrReadout= ((pNthAcq%2+1)==1 ? SRAM1_END_READOUT : SRAM2_END_READOUT);
    }



    void GlibInterface::Run(Glib& pGlib)
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

                        for(uint32_t i=0; i<cDataBitVector.size(); i++)
                        {
                            if(cDataBitVector[i])
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


    void GlibInterface::UpdateGlibWrite(Glib& pGlib,const std::string& pRegNode,const uint32_t& pVal)
    {
        ChooseBoard(pGlib.getBeId());

        WriteReg( pRegNode,pVal );
        pGlib.setReg( pRegNode,pVal );
    }


    void GlibInterface::UpdateGlibRead(Glib& pGlib,const std::string& pRegNode)
    {
        ChooseBoard(pGlib.getBeId());

        pGlib.setReg(pRegNode,(uint32_t) ReadReg(pRegNode));
    }



}
