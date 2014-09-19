#include "../HWDescription/Cbc.h"
#include "../HWDescription/Module.h"
#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../tools/Calibration.h"
#include <TApplication.h>

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;
using namespace Ph2_System;

void syntax(int argc){
    if(argc > 2) std::cerr << RED << "ERROR: Syntax: calibrationtest HWDescriptionFile" << std::endl;
    else return;
}

int main(int argc, char* argv[])
{
    syntax(argc);

    std::string cHWFile;
    if(argv[1] == "8CBC") cHWFile = "settings/HWDescription_8CBC.xml";
    else cHWFile = "settings/HWDescription_2CBC.xml";

    TApplication cApp("Root Application", &argc, argv);
    TQObject::Connect("TCanvas", "Closed()", "TApplication", &cApp, "Terminate()");
   
    Calibration cCalibration;
	cCalibration.InitializeHw(cHWFile);
	cCalibration.InitializeSettings(cHWFile);
    cCalibration.CreateResultDirectory("Results/Calibration");
    cCalibration.InitResultFile();
	cCalibration.InitialiseTestGroup();
    cCalibration.ConfigureHw();
    cCalibration.VplusScan();
    cCalibration.OffsetScan();
    cCalibration.SaveResults();

    cApp.Run();
}
