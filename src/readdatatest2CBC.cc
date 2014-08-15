/*

  FileName :                     readdatatest.cpp
  Content :
  Programmer :                   Nicolas PIERRE, Lorenzo Bidegain
  Version :                      0.3
  Date of creation :             11/08/14
  Support :                      mail to : nico.pierre@icloud.com, lorenzo.bidegain@cern.ch

*/

#include "../HWDescription/Cbc.h"
#include "../HWDescription/Module.h"
#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../HWInterface/Utilities.h"
#include <boost/format.hpp>
#include <TH1F.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TApplication.h>
#include <TROOT.h>

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;


int main(int argc, char* argv[])
{

    TApplication cApp("Root Application", &argc, argv);

    //Two Cbc
    Cbc cCbc_00(0,0,0,0,0,"settings/FE0CBC0Hole.txt");
    Cbc cCbc_01(0,0,0,0,1,"settings/FE0CBC1Hole.txt");

    //One Module
    Module cModule_00;
    cModule_00.fModuleId=0;
    cModule_00.addCbc(cCbc_00);
    cModule_00.addCbc(cCbc_01);

    //One Glib
    BeBoard* cGlib_00 = new BeBoard(0,0);
    cGlib_00->addModule(cModule_00);

    BeBoardFWInterface* cBeBoardFW;
    std::map<int8_t,BeBoardFWInterface*> cBeBoardFWMap;

    CbcInterface cCbcInterface(cBeBoardFWMap);
    BeBoardInterface cBeBoardInterface(cBeBoardFWMap);
    cBeBoardFW= new GlibFWInterface(UHAL_CONNECTION_FILE,0);
    cBeBoardFWMap[0]=cBeBoardFW;

    cBeBoardInterface.ConfigureBoard(cGlib_00);

    cCbcInterface.ConfigureCbc(cGlib_00->getModule(0)->getCbc(0));
    cCbcInterface.ConfigureCbc(cGlib_00->getModule(0)->getCbc(1));

    TCanvas *cCanvas = new TCanvas("Data Acq", "Different hits counters", 1000, 800);
    cCanvas->Divide(2,1);

    std::vector<TH1F*> cHistVec;
    for(uint8_t m=0; m<cGlib_00->getModule(0)->getNCbc(); m++)
    {
        cHistVec.push_back(new TH1F(Form("Histo_Hits_CBC%d",m), Form("Occupancy_CBC%d",m), 255, -0.5, 254.5));
    }

    uint32_t cNthAcq = 0;

    for(uint32_t p=100; p<140; p+=2)
    {
        cCbcInterface.WriteCbcReg(cGlib_00->getModule(0)->getCbc(0),"VCth",p);
        cCbcInterface.WriteCbcReg(cGlib_00->getModule(0)->getCbc(1),"VCth",p);

        uint32_t cNevents = 200;
        uint32_t cN = 0;

        for(uint8_t m=0; m<cGlib_00->getModule(0)->getNCbc(); m++)
        {
            delete cHistVec[m];
            cHistVec[m] = new TH1F(Form("Histo_Hits_CBC%d",m), Form("Occupancy_CBC%d",m), 255, -0.5, 254.5);
        }

        gStyle->SetOptStat(0);

        usleep( 100 );

        cBeBoardFWMap[0]->fStop = false;

        while(!(cBeBoardFWMap[0]->fStop))
        {

            cBeBoardInterface.Start(cGlib_00);
            cBeBoardInterface.ReadData(cGlib_00, cNthAcq, true );
            cBeBoardInterface.Stop(cGlib_00, cNthAcq );

            const Event *cEvent = cBeBoardFWMap[0]->fData->GetNextEvent();

            while( cEvent )
            {

                if( cNevents != 0 && cN == cNevents )
                {
                    cBeBoardFWMap[0]->fStop = true;
                    break;
                }

                for(uint8_t i=0; i<cGlib_00->getNFe(); i++)
                {

                    for(uint8_t m=0; m<cGlib_00->getModule(i)->getNCbc(); m++)
                    {
                        uint32_t cNHits = 0;
                        std::vector<bool> cDataBitVector = cEvent->DataBitVector(i,m);

                        for(uint8_t n=0; n<cDataBitVector.size(); n++)
                        {
                            if(cDataBitVector[n])
                            {
                                cNHits++;
                            }
                        }
                        cHistVec[m]->Fill(cNHits);
                    }
                }

                //cCanvas->Print(((boost::format("output/Histogram_Event_%d.pdf") %(cN)).str()).c_str());

                cEvent = cBeBoardFWMap[0]->fData->GetNextEvent();
                cN++;
            }

            for(uint8_t m=0; m<cHistVec.size(); m++)
            {
                cCanvas->cd(uint32_t(m)+1);
                cHistVec[m]->Draw();
            }

            cCanvas->Update();

            if( cN == cNevents )
            {
                cBeBoardFWMap[0]->fStop = true;
                break;
            }

            cNthAcq++;

        }

        cCanvas->Print(((boost::format("output/Histogram_Vcth_%d.pdf") %(p)).str()).c_str());

    }

    cApp.Run();

    return 0;
}
