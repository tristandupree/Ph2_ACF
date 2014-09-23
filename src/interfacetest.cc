#include "../HWDescription/Cbc.h"
#include "../HWDescription/Module.h"
#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../HWInterface/Utilities.h"
#include "../System/SystemController.h"
#include <boost/format.hpp>
#include <TH1F.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TApplication.h>
#include <TROOT.h>

#include <sys/time.h>
#include <ctime>

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;
using namespace Ph2_System;


uint64_t get_time()
{
    /* Linux */
     struct timeval tv;

     gettimeofday(&tv, NULL);

     uint64_t ret = tv.tv_usec;
     /* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
     ret /= 1000;

     /* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
     ret += (tv.tv_sec * 1000);

     return ret;
}

int main(int argc, char* argv[])
{

    SystemController cSystemController;

    cSystemController.InitializeHw(XML_DESCRIPTION_FILE_2CBC);
    cSystemController.ConfigureHw();

    uint64_t t0 = get_time();

    cSystemController.fCbcInterface->ConfigureCbc(cSystemController.fShelveVec[0]->getBoard(0)->getModule(0)->getCbc(0), true);

    uint64_t t1 = get_time();

    std::cout << "Time for configuring 1 CBC: " << t1-t0 << " milliseconds!" << std::endl;

    t0 = get_time();

    cSystemController.fCbcInterface->WriteCbcReg(cSystemController.fShelveVec[0]->getBoard(0)->getModule(0)->getCbc(0),"VCth",0x78, true);
    // cSystemController.fCbcInterface->WriteCbcReg(cSystemController.fShelveVec[0]->getBoard(0)->getModule(0)->getCbc(1),"VCth",0x78, false);

    t1 = get_time();

    std::cout << "Time for single transaction to 1 CBC: " << t1-t0 << " milliseconds!" << std::endl;

    std::vector< std::pair< std::string, uint8_t > > cRegVec;

    uint8_t nChannels = 255;
    for(uint8_t cChannel = 1; cChannel < nChannels; cChannel++){

        TString cRegName = Form("Channel%03d",cChannel);
        uint8_t cRegValue = 0x50;

        std::pair<std::string, uint8_t> cRegPair = std::make_pair(cRegName.Data(), cRegValue);
        cRegVec.push_back(cRegPair);
    }

    t0 = get_time();

    cSystemController.fCbcInterface->WriteCbcMultReg(cSystemController.fShelveVec[0]->getBoard(0)->getModule(0)->getCbc(0),cRegVec, true);
    // cSystemController.fCbcInterface->WriteCbcMultReg(cSystemController.fShelveVec[0]->getBoard(0)->getModule(0)->getCbc(1),cRegVec, false);

    t1 = get_time();

    std::cout << "Time for writing " << int(nChannels) << " registers to 1 CBC: " << t1-t0 << " milliseconds!" << std::endl;


    return 0;
}
