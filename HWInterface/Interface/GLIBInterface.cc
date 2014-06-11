/*

  FileName : 					GLIBInterface.cc
  Content : 					 GLIBInterface class, init/config of the Glib
  Programmer : 				  Nicolas PIERRE
  Version : 					 0.0
  Date of creation : 	        07/06/14
  Support : 					 mail to : nicolas.pierre@etu.unistra.net

*/

#include <uhal/uhal.hpp>
#include <boost/format.hpp>
#include <boost/date_time.hpp>
#include "GLIBInterface.h"

#define EVENT_SIZE_32    32

#define DEV_FLAG         0


namespace Ph2_HwInterface
{

    UInt_t GLIBInterface::NBe = 0;
    const unsigned int GLIBController::fPacketSize = EVENT_SIZE_32;

    GLIBInterface::GLIBInterface( const char *pConfigFile, int pCbcNb/*, GLIB pGLIB*/ ):
		fUhalConfigFileName( "/opt/testing/trackerDAQ-3.2/CBCDAQ/GlibSupervisor/xml/connections.xml" ),
		fGLIBId ("board"),
		fOutputDir("./"),
		fBeId(0),
		fNFe(1),
		fNCbc(pCbcNb),
		fNegativeLogicCBC(true),
		fStop(false)
    {
        if(DEV_FLAG)
            uhal::setLogLevelTo( uhal::Error() );
        else
            uhal::disableLogging();

        uhal::ConnectionManager cm( fUhalConfigFileName );
        fBoard = new uhal::HwInterface( cm.getDevice( "B" ) );

        //fGlibSettings = GLIB::GetGlibSettings(pGLIB);

        NBe++;
    }

    GLIBInterface::~GLIBInterface()
    {

    }

    void GLIBInterface::ConfigureGLIB()
    {
        //Primary Configuration

		WriteReg("user_wb_ttc_fmc_regs.pc_commands.SRAM1_end_readout",0);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands.SRAM2_end_readout",0);
        WriteReg("ctrl_sram.sram1_user_logic",1);
        WriteReg("ctrl_sram.sram2_user_logic",1);

		boost::this_thread::sleep(cPause);

        //GlibSetting : map<string,(u)int> created from GLIB class
		for(GlibSetting::iterator cIt = fGlibSetting.begin(); cIt != fGlibSetting.end(); cIt++ )
        {
			WriteReg( cIt->first , (uint32_t) cIt->second );
		}

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

		boost::this_thread::sleep( cPause * 3 );

        //Setting internal members
        fNFe = fGlibSetting.find( "FE_expected" )->second;
		fNFe = fNFe == 1 ? 1 : 2;

        unsigned int cExpectedCbc = fGlibSetting.find( "CBC_expected" )->second;
		fNCbc = cExpectedCbc == 1 ? 1 : 2;

		//Preparing CBC register setting and update list map
		fCbcRegSetting.Reset( fNFe, fNCbc );
		fCbcRegUpdateList.Reset( fNFe );
    }

    void GLIBInterface::Start()
    {
#ifdef __CBCDAQ_DEV__
		long mtime = getTimeTook( fStartVeto, 1 );
		std::cout << "Time took for the trigger veto to trigger enable: " << std::dec << mtime << " ms." << std::endl;
#endif

        WriteReg("break_trigger",0);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands.PC_config_ok",1);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands2.force_BG0_start",1);
    }

    void GLIBInterface::Stop( uint32_t pNthAcq )
    {
        uhal::ValWord<uint32_t> cVal;

        SRAMforDAQ( pNthAcq );

		WriteReg("break_trigger",1);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands.PC_config_ok",0);
        WriteReg("user_wb_ttc_fmc_regs.pc_commands2.force_BG0_start",0);

		do
        {
			cVal = ReadReg(fStrFull);

            if (cVal==1)
                boost::this_thread::sleep(cWait);

        } while (cVal==1);

        WriteReg(fStrReadout,0);
        fNTotalAcq++;
    }

    void Pause()
    {
        RegWrite("break_trigger",1);
#ifdef __CBCDAQ_DEV__
        std::cout << "Pause engaged" << std::endl;
#endif
    }

    void Unpause()
    {
        RegWrite("break_trigger",0);
#ifdef __CBCDAQ_DEV__
        std::cout << "Pause disengaged" << std::endl;
#endif
    }

    void ReadDAQ( unsigned int pNthAcq, bool pBreakTrigger )
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
		SRAMForDAQ( pNthAcq );

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
		fData = push_back(cData);

    }

    void SRAMforDAQ( uint32_t pNthAcq )
    {
        fStrSram  = (boost::format("sram%d") % (pNthAcq%2+1)).str();
		fStrSramUserLogic =  (boost::format("ctrl_sram.sram%d_user_logic") % (pNthAcq%2+1)).str();
		fStrFull = (boost::format("user_wb_ttc_fmc_regs.flags.SRAM%d_full") % (pNthAcq%2+1)).str();
		fStrReadout= (boost::format("user_wb_ttc_fmc_regs.pc_commands.SRAM%d_end_readout") % (pNthAcq%2+1)).str();
    }
}
