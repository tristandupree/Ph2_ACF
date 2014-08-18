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

    SystemController cSystemController;

    cSystemController.InitializeHw(XML_DESCRIPTION_FILE);
    cSystemController.ConfigureHw();

    TCanvas *cCanvas = new TCanvas("Data Acq", "Different hits counters", 1000, 800);
    cCanvas->Divide(2,1);

    std::vector<TH1F*> cHistVec;
    for(uint8_t cNCbc=0; cNCbc<cSystemController.fShelveVec[0]->getBoard(0)->getModule(0)->getNCbc(); cNCbc++)
    {
        cHistVec.push_back(new TH1F(Form("Histo_Hits_CBC%d",cNCbc), Form("Occupancy_CBC%d",cNCbc), 255, -0.5, 254.5));
    }

    uint32_t cNthAcq = 0;

    for(uint32_t p=100; p<140; p+=2)
    {
        cCbcInterface.WriteCbcReg(cSystemController.fShelveVec[0]->getBoard(0)->getModule(0)->getCbc(0),"VCth",p);
        cCbcInterface.WriteCbcReg(cSystemController.fShelveVec[0]->getBoard(0)->getModule(0)->getCbc(1),"VCth",p);

        uint32_t cNevents = 200;
        uint32_t cN = 0;

        for(uint8_t cNCbc=0; cNCbc<cSystemController.fShelveVec[0]->getBoard(0)->getModule(0)->getNCbc(); cNCbc++)
        {
            delete cHistVec[cNCbc];
            cHistVec[cNCbc] = new TH1F(Form("Histo_Hits_CBC%d",cNCbc), Form("Occupancy_CBC%d",cNCbc), 255, -0.5, 254.5);
        }

        gStyle->SetOptStat(0);

        usleep( 100 );

        cBeBoardFWMap[0]->fStop = false;

        while(!(cBeBoardFWMap[0]->fStop))
        {

            cBeBoardInterface.Start(cSystemController.fShelveVec[0]->getBoard(0));
            cBeBoardInterface.ReadData(cSystemController.fShelveVec[0]->getBoard(0), cNthAcq, true );
            cBeBoardInterface.Stop(cSystemController.fShelveVec[0]->getBoard(0), cNthAcq );

            const Event *cEvent = cBeBoardFWMap[0]->fData->GetNextEvent();

            while( cEvent )
            {

                if( cNevents != 0 && cN == cNevents )
                {
                    cBeBoardFWMap[0]->fStop = true;
                    break;
                }

                for(uint8_t cNFe=0; cNFe<cSystemController.fShelveVec[0]->getBoard(0)->getNFe(); cNFe++)
                {

                    for(uint8_t cNCbc=0; cNCbc<cSystemController.fShelveVec[0]->getBoard(0)->getModule(cNFe)->getNCbc(); cNCbc++)
                    {
                        uint32_t cNHits = 0;
                        std::vector<bool> cDataBitVector = cEvent->DataBitVector(cNFe,cNCbc);

                        for(uint8_t cDBVec=0; n<cDataBitVector.size(); n++)
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
