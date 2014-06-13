/*

  FileName : 					GLIBInterface.cc
  Content : 					 GLIBInterface class, init/config of the Glib
  Programmer : 				  Nicolas PIERRE
  Version : 					 1.1
  Date of creation : 	        07/06/14
  Support : 					 mail to : nicolas.pierre@etu.unistra.net

*/

#include <uhal/uhal.hpp>
#include <boost/format.hpp>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <time.h>
#include "GLIBInterface.h"

#define EVENT_SIZE_32    32

#define DEV_FLAG         0


namespace Ph2_HwInterface
{

    unsigned int GLIBInterface::NBe = 0;
    unsigned int GLIBInterface::fPacketSize = EVENT_SIZE_32;

    //Constructor, make the connection to the board and get settings from GLIB
    GLIBInterface::GLIBInterface(const char *puHalConfigFileName, const char *pBoardId/*, Glib pGLIB*/ ):
        RegManager(puHalConfigFileName,pBoardId),
        //fGlib(pGlib),
		fOutputDir("./"),
		fBeId(0),
		fNFe(1),
		fNCbc(2),
		fNegativeLogicCBC(true),
		fStop(false)
    {
        //fGlibSettings = Glib::GetGlibSettings(pGlib);

        NBe++;
    }

    GLIBInterface::~GLIBInterface()
    {

    }

    void GLIBInterface::ConfigureGLIB()
    {
        boost::posix_time::milliseconds cPause(200);

        //Primary Configuration
		WriteReg("user_wb_ttc_fmc_regs.pc_commands.SRAM1_end_readout",0);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands.SRAM2_end_readout",0);
        WriteReg("ctrl_sram.sram1_user_logic",1);
        WriteReg("ctrl_sram.sram2_user_logic",1);

		boost::this_thread::sleep(cPause);

        /*
        //GlibSetting : map<string,(u)int> created from GLIB class
		for(GlibSetting::iterator cIt = fGlibSetting.begin(); cIt != fGlibSetting.end(); cIt++ )
        {
			WriteReg( cIt->first , (uint32_t) cIt->second );
		}
        */

        //Temporary hardcoding
        WriteReg("CBC_expected",3);
        WriteReg("COMMISSIONNING_MODE_CBC_TEST_PULSE_VALID",1);
        WriteReg("COMMISSIONNING_MODE_DELAY_AFTER_FAST_RESET",50);
        WriteReg("COMMISSIONNING_MODE_DELAY_AFTER_L1A",400);
        WriteReg("COMMISSIONNING_MODE_DELAY_AFTER_TEST_PULSE",201);
        WriteReg("COMMISSIONNING_MODE_RQ",1);
        WriteReg("FE_expected",1);
        WriteReg("cbc_stubdata_latency_adjust_fe1",1);
        WriteReg("cbc_stubdata_latency_adjust_fe2",1);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands.ACQ_MODE",1);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands.CBC_DATA_GENE",1);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands.CBC_DATA_PACKET_NUMBER",100);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands.INT_TRIGGER_FREQ",4);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands.TRIGGER_SEL",0);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands2.clock_shift",0);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands2.negative_logic_CBC",1);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands2.negative_logic_sTTS",0);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands2.polarity_tlu",0);
        //End of temporary hardcoding


        WriteReg("user_wb_ttc_fmc_regs.pc_commands.SPURIOUS_FRAME",0);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands2.force_BG0_start",0);
        WriteReg("user_wb_ttc_fmc_regs.cbc_acquisition.CBC_TRIGGER_ONE_SHOT",0);
        WriteReg("break_trigger",1);

        WriteReg("user_wb_ttc_fmc_regs.pc_commands.PC_config_ok",0);
		WriteReg("user_wb_ttc_fmc_regs.pc_commands.SRAM1_end_readout",0);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands.SRAM2_end_readout",0);
        WriteReg("ctrl_sram.sram1_user_logic",1);
        WriteReg("ctrl_sram.sram2_user_logic",1);

		boost::this_thread::sleep(cPause);

		WriteReg("user_wb_ttc_fmc_regs.pc_commands.SPURIOUS_FRAME",0);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands2.force_BG0_start",0);
        WriteReg("user_wb_ttc_fmc_regs.cbc_acquisition.CBC_TRIGGER_ONE_SHOT",0);
        WriteReg("break_trigger",1);

		boost::this_thread::sleep( cPause*3 );


        //Setting internal members
        //fNFe = fGlibSetting.find( "FE_expected" )->second;
		fNFe = fNFe == 1 ? 1 : 2;

        //unsigned int cExpectedCbc = fGlibSetting.find( "CBC_expected" )->second;
		fNCbc = /*cExpectedCbc == 1 ? 1 :*/ 2;

    }


    void GLIBInterface::Start()
    {
#ifdef __CBCDAQ_DEV__
		long mtime = getTimeTook( fStartVeto, 1 );
		std::cout << "Time took for the trigger veto to trigger enable: " << std::dec << mtime << " ms." << std::endl;
#endif

        //Starting the DAQ
        WriteReg("break_trigger",0);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands.PC_config_ok",1);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands2.force_BG0_start",1);
    }

    void GLIBInterface::Stop( uint32_t pNthAcq )
    {
        uhal::ValWord<uint32_t> cVal;

        //Select SRAM
        SRAMforDAQ( pNthAcq );

        //Stop the DAQ
		WriteReg("break_trigger",1);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands.PC_config_ok",0);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands2.force_BG0_start",0);

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

    void GLIBInterface::Pause()
    {
        WriteReg("break_trigger",1);
#ifdef __CBCDAQ_DEV__
        std::cout << "Pause engaged" << std::endl;
#endif
    }

    void GLIBInterface::Unpause()
    {
        WriteReg("break_trigger",0);
#ifdef __CBCDAQ_DEV__
        std::cout << "Pause disengaged" << std::endl;
#endif
    }

    void GLIBInterface::ReadDAQ( unsigned int pNthAcq, bool pBreakTrigger )
    {

#ifdef __CBCDAQ_DEV__
        std::cout << "ReadDataInSRAM" << std::endl;

		struct timeval cStartReadDataInSRAM, cStartBlockRead;
		struct timeval start;
		long mtime;

		gettimeofday( &cStartReadDataInSRAM, 0 );
		gettimeofday(&start, 0);
#endif

		//Readout settings
		boost::posix_time::milliseconds cWait(1);

		uhal::ValWord<uint32_t> cVal;
		uint32_t cNPackets= fNeventPerAcq+1;
		uint32_t cBlockSize = cNPackets * fPacketSize;

		//Wait for start acknowledge
		do
        {
			cVal=ReadReg("user_wb_ttc_fmc_regs.status_flags.CMD_START_VALID");

			if ( cVal==0 )
				boost::this_thread::sleep(cWait);

		} while ( cVal==0 );

#ifdef __CBCDAQ_DEV__
		mtime = getTimeTook( start, 1 );
		std::cout << "BeController::ReadDataInSRAM()  Time took for the CMD_START_VALID flag to be set: " << std::dec << mtime << " ms." << std::endl;
#endif

		//FIFO goes to write_data state
		//Select SRAM
		SRAMforDAQ( pNthAcq );

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
			WriteReg("break_trigger",1);
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
		fData = cData;

    }

    void GLIBInterface::SRAMforDAQ( uint32_t pNthAcq )
    {
        fStrSram  = (boost::format("sram%d") % (pNthAcq%2+1)).str();
		fStrSramUserLogic =  (boost::format("ctrl_sram.sram%d_user_logic") % (pNthAcq%2+1)).str();
		fStrFull = (boost::format("user_wb_ttc_fmc_regs.flags.SRAM%d_full") % (pNthAcq%2+1)).str();
		fStrReadout= (boost::format("user_wb_ttc_fmc_regs.pc_commands.SRAM%d_end_readout") % (pNthAcq%2+1)).str();
    }

    /*
    void GLIBInterface::UpdateReg( const std::string& pRegNode, const uint32_t& pVal )
    {
        Glib.SetReg( const std::string& pRegNode, const uint32_t& pVal );
    }
    */
}
