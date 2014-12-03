#include <cstring>
#include "../HWDescription/Cbc.h"
#include "../HWDescription/Module.h"
#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../tools/Calibration.h"
//#include "TROOT.h"
#include <TApplication.h>
#include "../Utils/argvparser.h"


using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;
using namespace Ph2_System;

using namespace CommandLineProcessing;


int main( int argc, char* argv[] )
{
	ArgvParser cmd;

	// init
	cmd.setIntroductoryDescription( "CMS Ph2_ACF  calibration routine using K. Uchida's algorithm "/*or a fast algoriithm*/ );
	// error codes
	cmd.addErrorCode( 0, "Success" );
	cmd.addErrorCode( 1, "Error" );
	// options
	cmd.setHelpOption( "h", "help", "Print this help page" );

	cmd.defineOption( "file", "Hw Description File . Default value: settings/Calibration2CBC.xml", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "file", "f" );

	cmd.defineOption( "output", "Output Directory . Default value: Results", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "output", "o" );

	cmd.defineOption( "skip", "skip scaning VCth vs Vplus", ArgvParser::NoOptionAttribute );

	// cmd.defineOption( "fast", "Use fast calibration algorithm", ArgvParser::NoOptionAttribute );

	int result = cmd.parse( argc, argv );
	if ( result != ArgvParser::NoParserError )
	{
		std::cout << cmd.parseErrorDescription( result );
		exit( 1 );
	}

	// now query the parsing results
	std::string cHWFile = ( cmd.foundOption( "file" ) ) ? cmd.optionValue( "file" ) : "settings/Calibration2CBC.xml";
	std::string cDirectory = ( cmd.foundOption( "output" ) ) ? cmd.optionValue( "output" ) : "Results/";
	cDirectory += "Calibration";
	bool cVplus = ( cmd.foundOption( "skip" ) ) ? true : false;
	// bool cFast = ( cmd.foundOption( "fast" ) ) ? true : false;

  //      gROOT->SetBatch(true);
	TApplication cApp( "Root Application", &argc, argv );
	TQObject::Connect( "TCanvas", "Closed()", "TApplication", &cApp, "Terminate()" );

	// if ( cFast )
	// {
	//  FastCalibration cCalibration;
	// }
	// else
	// {
	Calibration cCalibration;
	cCalibration.InitializeHw( cHWFile );
	cCalibration.InitializeSettings( cHWFile );
	cCalibration.CreateResultDirectory( cDirectory );
	cCalibration.InitResultFile( "CalibrationResults" );
	cCalibration.InitialiseTestGroup();
	cCalibration.ConfigureHw();
	if ( !cVplus ) cCalibration.VplusScan();
	cCalibration.OffsetScan();
	cCalibration.SaveResults();
	// }
	cApp.Run();

	return 0;
}
