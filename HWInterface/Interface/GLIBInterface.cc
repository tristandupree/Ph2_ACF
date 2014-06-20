/*

    FileName :                    GlibInterface.cc
    Content :                     GlibInterface class, init/config of the Glib
    Programmer :                  Nicolas PIERRE
    Version :                     1.3
    Date of creation :            07/06/14
    Support :                     mail to : nicolas.pierre@cern.ch

*/

#include <uhal/uhal.hpp>
#include <boost/format.hpp>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <time.h>
#include "GLIBInterface.h"
#include "Utilities.h"
#include "../../HWDescription/Description/Definition.h"

#define DEV_FLAG         0


namespace Ph2_HwInterface
{

    ///Constructor, makes the board map
    GlibInterface::GlibInterface(const char *puHalConfigFileName):
        RegManager(puHalConfigFileName)
		///fNegativeLogicCBC(true),
		///fStop(false)
    {

    }


    GlibInterface::~GlibInterface()
    {

    }


    void GlibInterface::ConfigureGlib(Glib& pGlib)
    {

        ChooseBoard(pGlib.getBeId());

        boost::posix_time::milliseconds cPause(200);

        ///Primary Configuration
		WriteReg(SRAM1_END_READOUT,0);
        WriteReg(SRAM2_END_READOUT,0);
        WriteReg(SRAM1_USR_LOGIC,1);
        WriteReg(SRAM2_USR_LOGIC,1);

		boost::this_thread::sleep(cPause);


        /*
        GlibRegMap : map<std::string,uint8_t> created from Glib class

        Mandatory to go through a created cGlibRegMap.
        If you want to put directly pGlib.getGlibRegMap(), you'll end up with
        a seg fault error, as it is not putting all the map in mem but only
        begin() and end().
        */

        GlibRegMap cGlibRegMap = pGlib.getGlibRegMap();
		for(GlibRegMap::iterator cIt = cGlibRegMap.begin(); cIt != cGlibRegMap.end(); cIt++ )
        {
			WriteReg( cIt->first , (uint32_t) cIt->second );
		}


        WriteReg(SPURIOUS_FRAME,0);
        WriteReg(FORCE_BG0_START,0);
        WriteReg(CBC_TRIGGER_1SHOT,0);
        WriteReg(BREAK_TRIGGER,1);

        WriteReg(PC_CONFIG_OK,0);
		WriteReg(SRAM1_END_READOUT,0);
        WriteReg(SRAM2_END_READOUT,0);
        WriteReg(SRAM1_USR_LOGIC,1);
        WriteReg(SRAM2_USR_LOGIC,1);

		boost::this_thread::sleep(cPause);

		WriteReg(SPURIOUS_FRAME,0);
        WriteReg(FORCE_BG0_START,0);
        WriteReg(CBC_TRIGGER_1SHOT,0);
        WriteReg(BREAK_TRIGGER,1);

		boost::this_thread::sleep( cPause*3 );

    }


    void GlibInterface::Start(Glib& pGlib)
    {

#ifdef __CBCDAQ_DEV__
		long mtime = getTimeTook( fStartVeto, 1 );
		std::cout << "Time took for the trigger veto to trigger enable: " << std::dec << mtime << " ms." << std::endl;
#endif

        ChooseBoard(pGlib.getBeId());

        ///Starting the DAQ
        WriteReg(BREAK_TRIGGER,0);
        WriteReg(PC_CONFIG_OK,1);
        WriteReg(FORCE_BG0_START,1);
    }


    void GlibInterface::Stop(Glib& pGlib, uint32_t pNthAcq )
    {

        uhal::ValWord<uint32_t> cVal;

        ChooseBoard(pGlib.getBeId());

        ///Select SRAM
        SelectSRAM( pNthAcq );

        ///Stop the DAQ
		WriteReg(BREAK_TRIGGER,1);
        WriteReg(PC_CONFIG_OK,0);
        WriteReg(FORCE_BG0_START,0);

        boost::posix_time::milliseconds cWait(100);

        ///Wait for the selected SRAM to be full then empty it
		do
        {
			cVal = ReadReg(fStrFull);

            if (cVal==1)
                boost::this_thread::sleep(cWait);

        } while (cVal==1);

        WriteReg(fStrReadout,0);
        ///fNTotalAcq++;
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

		///Readout settings
		boost::posix_time::milliseconds cWait(1);

		uhal::ValWord<uint32_t> cVal;
		uint32_t cNPackets= EVENT_NUMBER+1;
		uint32_t cBlockSize = cNPackets * PACKET_SIZE;

		///Wait for start acknowledge
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

		///FIFO goes to write_data state
		///Select SRAM
		SelectSRAM( pNthAcq );

#ifdef __CBCDAQ_DEV__
		gettimeofday(&start, 0);
#endif

		///Wait for the SRAM full condition.
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

		///break trigger
		if( pBreakTrigger )
        {
			WriteReg(BREAK_TRIGGER,1);
		}

        /// JRF end
#ifdef __CBCDAQ_DEV__
		gettimeofday( &fStartVeto, 0 );
#endif

		///Set read mode to SRAM
		WriteReg(fStrSramUserLogic,0);

#ifdef __CBCDAQ_DEV__
	    gettimeofday( &cStartBlockRead, 0 );
#endif

		///Read SRAM
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

		///Wait for the non SRAM full condition starts,
		do
        {
			cVal = ReadReg(fStrFull);

			if (cVal==1)
				boost::this_thread::sleep(cWait);

		} while (cVal==1);

		///Wait for the non SRAM full condition ends.
#ifdef __CBCDAQ_DEV__
		mtime = getTimeTook( start, 0 );
		std::cout << "Time took to the full flag to be 0 : " << std::dec << mtime << " us." << std::endl;
#endif

		WriteReg(fStrReadout,0);

#ifdef __CBCDAQ_DEV__
		mtime = getTimeTook( cStartReadDataInSRAM, 1 );
		std::cout << "Time took for ReadDataInSRAM: " << std::dec << mtime << " ms." << std::endl;
#endif

        ///One data for one event --> Enhanced later
		fData = cData;

    }


    void GlibInterface::SelectSRAM(uint32_t pNthAcq)
    {
        fStrSram  = ((pNthAcq%2+1)==1 ? SRAM1 : SRAM2);
        fStrSramUserLogic =  ((pNthAcq%2+1)==1 ? SRAM1_USR_LOGIC : SRAM2_USR_LOGIC);
        fStrFull = ((pNthAcq%2+1)==1 ? SRAM1_FULL : SRAM2_FULL);
        fStrReadout= ((pNthAcq%2+1)==1 ? SRAM1_END_READOUT : SRAM2_END_READOUT);
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
