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

int main(int argc, char* argv[])
{
    Calibration cCalibration("testfile.root");
    TApplication cApp("Root Application", &argc, argv);
    // TQObject::Connect("TCanvas", "Closed()", "TApplication", &cApp, "Terminate()");

	cCalibration.InitializeHw("settings/HWDescription_2CBC.xml");
	cCalibration.InitializeSettings("settings/HWDescription_2CBC.xml");
	cCalibration.InitialiseTestGroup();
    cCalibration.ConfigureHw();
    // cCalibration.VplusScan();
    cCalibration.OffsetScan();
    cApp.Run();
}
