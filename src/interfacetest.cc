#include "../HWDescription/Cbc.h"
#include "../HWDescription/Module.h"
#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../Utils/Utilities.h"
#include "../System/SystemController.h"
#include <boost/format.hpp>
#include <TH1F.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TApplication.h>
#include <TROOT.h>

#include <sys/time.h>
#include <ctime>
#include "../Utils/Visitor.h"
#include "../Utils/argvparser.h"


using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;
using namespace Ph2_System;

using namespace CommandLineProcessing;


uint64_t get_time()
{
	/* Linux */
	struct timeval tv;

	gettimeofday( &tv, NULL );

	uint64_t ret = tv.tv_usec;
	/* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
	ret /= 1000;

	/* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
	ret += ( tv.tv_sec * 1000 );

	return ret;
}

int main( int argc, char* argv[] )
{

	ArgvParser cmd;

	// init
	cmd.setIntroductoryDescription( "CMS Ph2_ACF  HWInterface performance Benchmark test" );
	// error codes
	cmd.addErrorCode( 0, "Success" );
	cmd.addErrorCode( 1, "Error" );
	// options
	cmd.setHelpOption( "h", "help", "Print this help page" );
	cmd.defineOption( "file", "Hw Description File . Default value: settings/HWDescription_2CBC.xml", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "file", "f" );
	cmd.defineOption( "configure", "test Configure HW", ArgvParser::NoOptionAttribute );
	cmd.defineOptionAlternative( "configure", "c" );
	cmd.defineOption( "single", "test single Register transaction", ArgvParser::NoOptionAttribute );
	cmd.defineOptionAlternative( "single", "s" );
	cmd.defineOption( "multi", "test multi-Register transaction", ArgvParser::NoOptionAttribute );
	cmd.defineOptionAlternative( "multi", "m" );

	int result = cmd.parse( argc, argv );
	if ( result != ArgvParser::NoParserError )
	{
		std::cout << cmd.parseErrorDescription( result );
		exit( 1 );
	}

	// now query the parsing results
	std::string cHWFile = ( cmd.foundOption( "file" ) ) ? cmd.optionValue( "file" ) : "settings/HWDescription_2CBC.xml";
	bool cConfigure = ( cmd.foundOption( "configure" ) ) ? true : false;
	bool cSingle = ( cmd.foundOption( "single" ) ) ? true : false;
	bool cMulti = ( cmd.foundOption( "multi" ) ) ? true : false;

	SystemController cSystemController;

	cSystemController.InitializeHw( cHWFile );
	cSystemController.InitializeSettings( cHWFile );
	if ( cConfigure )
	{
		uint64_t t0 = get_time();
		cSystemController.ConfigureHw();
		uint64_t t1 = get_time();
		std::cout << "Time for configuring all HW: " << t1 - t0 << " milliseconds!" << std::endl;
	}

	struct CbcWriter : public HwDescriptionVisitor
	{
		CbcInterface* fInterface;
		CbcWriter( CbcInterface* pInterface ): fInterface( pInterface ) {
		}

		void visit( Cbc& pCbc ) {
			fInterface->ConfigureCbc( &pCbc , true );
		}
	};

	if ( cSingle )
	{
		uint64_t t0 = get_time();

		CbcWriter pWriter( cSystemController.fCbcInterface );
		cSystemController.accept( pWriter );

		uint64_t t1 = get_time();

		std::cout << "Time for configuring all CBCs using visitor: " << t1 - t0 << " milliseconds!" << std::endl;
	}


	std::vector< std::pair< std::string, uint8_t > > cRegVec;

	uint8_t nChannels = 255;
	for ( uint8_t cChannel = 1; cChannel < nChannels; cChannel++ )
	{

		TString cRegName = Form( "Channel%03d", cChannel );
		uint8_t cRegValue = 0x50;

		std::pair<std::string, uint8_t> cRegPair = std::make_pair( cRegName.Data(), cRegValue );
		cRegVec.push_back( cRegPair );
	}

	if ( cMulti )
	{

		uint64_t t0 = get_time();

		cSystemController.fCbcInterface->WriteCbcMultReg( cSystemController.fShelveVector[0]->getBoard( 0 )->getModule( 0 )->getCbc( 0 ), cRegVec, true );
		// cSystemController.fCbcInterface->WriteCbcMultReg(cSystemController.fShelveVector[0]->getBoard(0)->getModule(0)->getCbc(1),cRegVec, false);

		uint64_t t1 = get_time();
		std::cout << "Time for writing " << int( nChannels ) << " registers to 1 CBC: " << t1 - t0 << " milliseconds!" << std::endl;

	}
	return 0;
}
