#include <cstring>
#include "../HWDescription/Cbc.h"
#include "../HWDescription/Module.h"
#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../tools/Calibration.h"
#include <TApplication.h>
#include <inttypes.h>
#include "../Utils/argvparser.h"


using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;
using namespace Ph2_System;

using namespace CommandLineProcessing;


void syntax( int argc )
{
	if ( argc > 4 ) std::cerr << RED << "ERROR: Syntax: calibrationtest VCth NEvents (HWDescriptionFile)" << std::endl;
	else if ( argc < 3 ) std::cerr << RED << "ERROR: Syntax: calibrationtest VCth NEvents (HWDescriptionFile)" << std::endl;
	else return;
}

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

	int pEventsperVcth;
	int cVcth;

	SystemController cSystemController;
	ArgvParser cmd;

	// init
	cmd.setIntroductoryDescription( "CMS Ph2_ACF  Data acquisition test and Data dump" );
	// error codes
	cmd.addErrorCode( 0, "Success" );
	cmd.addErrorCode( 1, "Error" );
	// options
	cmd.setHelpOption( "h", "help", "Print this help page" );

	cmd.defineOption( "file", "Hw Description File . Default value: settings/HWDescription_2CBC.xml", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "file", "f" );

	cmd.defineOption( "vcth", "Threshold in VCth units (hex (including 0x) or decimal) . Default values from HW description .XML file", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "vcth", "v" );

	cmd.defineOption( "events", "Number of Events . Default value: 10", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "events", "e" );

	int result = cmd.parse( argc, argv );
	if ( result != ArgvParser::NoParserError )
	{
		std::cout << cmd.parseErrorDescription( result );
		exit( 1 );
	}

	// now query the parsing results
	std::string cHWFile = ( cmd.foundOption( "file" ) ) ? cmd.optionValue( "file" ) : "settings/HWDescription_2CBC.xml";
	cVcth = ( cmd.foundOption( "vcth" ) ) ? cSystemController.convertAnyInt( cmd.optionValue( "vcth" ).c_str() ) : 0;
	pEventsperVcth = ( cmd.foundOption( "events" ) ) ? cSystemController.convertAnyInt( cmd.optionValue( "events" ).c_str() ) : 10;

	cSystemController.InitializeHw( cHWFile );
	cSystemController.ConfigureHw();

	if ( cVcth != 0 )
	{
		for ( auto cShelve : cSystemController.fShelveVector )
		{
			for ( auto cBoard : ( cShelve )->fBoardVector )
			{
				for ( auto cFe : cBoard.fModuleVector )
				{
					for ( auto cCbc : cFe.fCbcVector )
						cSystemController.fCbcInterface->WriteCbcReg( &cCbc, "VCth", uint8_t( cVcth ) );
				}
			}
		}
	}


	uint32_t cN = 0;
	uint32_t cNthAcq = 0;

	while ( cN < pEventsperVcth )
	{
		if ( cN == pEventsperVcth ) break;
		BeBoard pBoard = cSystemController.fShelveVector.at( 0 )->fBoardVector.at( 0 );
		cSystemController.Run( &pBoard, cNthAcq );

		const Event* cEvent = cSystemController.fBeBoardInterface->GetNextEvent( &pBoard );

		while ( cEvent )
		{
			std::cout << " cVcth = " << uint32_t( cVcth ) << std::endl;
			std::cout << ">>> Event #" << cN << std::endl;
			std::cout << *cEvent << std::endl;
			if ( cN == pEventsperVcth )
				break;
			cN++;

			if ( cN < pEventsperVcth )
				cEvent = cSystemController.fBeBoardInterface->GetNextEvent( &pBoard );
			else break;
		}
		cNthAcq++;
	}


}
