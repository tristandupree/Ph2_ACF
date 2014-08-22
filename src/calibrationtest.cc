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
    Calibration cCalibration;
    TApplication cApp("Root Application", &argc, argv);

	cCalibration.InitializeHw(XML_DESCRIPTION_FILE_8CBC);
    cCalibration.ConfigureHw();
    cCalibration.VplusScan();

    cApp.Run();
}
