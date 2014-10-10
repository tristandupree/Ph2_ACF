#include "datatestworker.h"
#include <QDebug>
#include <QVector>
#include <QThread>

#include <QEventLoop>
#include <QTimer>

#include <vector>

#include <boost/format.hpp>

#include <TH1.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TApplication.h>
#include <TROOT.h>
#include "utils.h"


using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;


namespace GUI
{
    DataTestWorker::DataTestWorker(QObject *parent,
                                   SystemController &sysController) ://, SystemController &sysCtrl) :
        QObject(parent),
        m_systemController(sysController)
    {
    }

    DataTestWorker::~DataTestWorker()
    {
        qDebug() << "Destructing " << this;
    }

    void DataTestWorker::requestWork()
    {
        mutex.lock();
        _working = true;
        _abort = false;
        qDebug()<<"Request worker start in Thread "<<thread()->currentThreadId();
        mutex.unlock();

        emit workRequested();
    }

    void DataTestWorker::abort()
    {
        mutex.lock();
        if (_working) {
            _abort = true;
            qDebug()<<"Request worker aborting in Thread "<<thread()->currentThreadId();
        }
        mutex.unlock();
    }

    void DataTestWorker::doWork()
    {
        //gROOT->SetBatch(kTRUE);
        static Int_t HistoID = 1;
        qDebug() << "in Testing env ";
        std::vector<TH1D*> graphs;
        std::vector<TCanvas*> vCanvas;

        TString name("h1_");
        Bool_t build = false;

        for (int i = 0; i <2 ; i++)
        {
            TCanvas *cCanvas = new TCanvas(build);
            //TCanvas *containerCanvas = new TCanvas(build);

            //containerCanvas->Divide(2,3);

            name += HistoID++;

            vCanvas.push_back(cCanvas);
            //vCanvas.at(i)->cd();


            TH1D *h1 = new TH1D(name.Data(),name.Data(),10,0, 10);
            graphs.push_back(h1);
            graphs.at(i)->Fill(i);
            graphs.at(i)->Draw();



            vCanvas.at(i)->Draw();
            qDebug() << "Here";

        }

        /*void testme() {
        TCanvas* contentCanvas = new TCanvas();
        TH1D* myHisto = new TH1D("myHisto", "my histogram", 10, 0.5, 10.5);
        TCanvas* containerCanvas = new TCanvas();
        containerCanvas->Divide(2,3);
        contentCanvas->cd();
        myHisto->Draw();
        containerCanvas->GetPad(3)->cd();
        contentCanvas->DrawClonePad();
        }*/


        emit sendGraphData(vCanvas); //void sendGraphData(const std::vector<TCanvas*> &canvas);

        // Set _working to false, meaning the process can't be aborted anymore.
        mutex.lock();
        _working = false;
        mutex.unlock();



        qDebug()<<"Worker process finished in Thread "<<thread()->currentThreadId();

        emit finished();
    }



    void DataTestWorker::ReadDataTest()
    {
        // Checks if the process should be aborted
        mutex.lock();
        bool abort = _abort;
        mutex.unlock();

        Bool_t build = false;
        std::vector<TCanvas*> vecCanvas;

        std::vector<std::shared_ptr<TCanvas>> vCanvas;
        std::vector<std::unique_ptr<TCanvas>> v2Canvas;

        for ( uint8_t cNCbc = 0; cNCbc < m_systemController.m_SystemControllerWorker->fShelveVec[0]->getBoard( 0 )->getModule( 0 )->getNCbc(); cNCbc++ )
        {
            cHistVec.push_back( new TH1F( Form( "Histo_Hits_CBC%d", cNCbc ), Form( "Occupancy_CBC%d", cNCbc ), 255, -0.5, 254.5 ) );
            //v2Canvas.push_back();
            //vecCanvas.push_back(new TCanvas(build));
        }

        uint32_t cNthAcq = 0;

        for (uint32_t cVCth = 100; cVCth<140; cVCth+=2)
        {

            m_systemController.m_SystemControllerWorker.fCbcInterface->WriteCbcReg( m_systemController.m_SystemControllerWorker.fShelveVec[0]->getBoard( 0 )->getModule( 0 )->getCbc( 0 ), "VCth", cVCth );
            m_systemController.m_SystemControllerWorker.fCbcInterface->WriteCbcReg( m_systemController.m_SystemControllerWorker.fShelveVec[0]->getBoard( 0 )->getModule( 0 )->getCbc( 1 ), "VCth", cVCth );
            if (abort)
            {
                qDebug()<<"Aborting worker process in Thread "<<thread()->currentThreadId();
                break;
            }

            for ( uint8_t cNCbc = 0; cNCbc < m_systemController.m_SystemControllerWorker.fShelveVec[0]->getBoard( 0 )->getModule( 0 )->getNCbc(); cNCbc++ )
            {
                delete cHistVec[cNCbc];
                cHistVec[cNCbc] = new TH1F( Form( "Histo_Hits_CBC%d", cNCbc ), Form( "Occupancy_CBC%d", cNCbc ), 255, -0.5, 254.5 );
                std::shared_ptr<TCanvas> test(new TCanvas(build));
                //vecCanvas.push_back(test);
            }

            uint32_t cNevents = 200;
            uint32_t cN = 0;

            while ( !( cN == cNevents ) )
            {
                Run(m_systemController.m_SystemControllerWorker.fShelveVec[0]->getBoard(0), cNthAcq);
                const Event* cEvent = m_systemController.m_SystemControllerWorker.fBeBoardInterface->GetNextEvent( m_systemController.m_SystemControllerWorker.fShelveVec[0]->getBoard( 0 ) );
                while ( cEvent )
                {
                    if ( cNevents != 0 && cN == cNevents )
                        break;

                    for ( uint8_t cNFe = 0; cNFe < m_systemController.m_SystemControllerWorker.fShelveVec[0]->getBoard( 0 )->getNFe(); cNFe++ )
                    {
                        for ( uint8_t cNCbc = 0; cNCbc < m_systemController.m_SystemControllerWorker.fShelveVec[0]->getBoard( 0 )->getModule( 0 )->getNCbc(); cNCbc++ )
                        {
                            uint cNHits = 0;
                            std::vector<bool> cDataBitVector = cEvent->DataBitVector( cNFe, cNCbc );

                            for ( uint8_t cDBVec = 0; cDBVec < cDataBitVector.size(); cDBVec++ )
                            {
                                if ( cDataBitVector[cDBVec] )
                                    cNHits++;
                            }
                            if (abort)
                            {
                                qDebug()<<"Aborting worker process in Thread "<<thread()->currentThreadId();
                                return;
                            }

                            cHistVec.at(cNCbc)->Fill( cNHits );
                        }
                    }

                    cEvent = m_systemController.m_SystemControllerWorker.fBeBoardInterface->GetNextEvent( m_systemController.m_SystemControllerWorker.fShelveVec[0]->getBoard( 0 ) );
                    cN++;
                }

                for ( uint8_t cNCbc = 0; cNCbc < cHistVec.size(); cNCbc++ )
                {
                    qDebug() << vecCanvas.size();
                    vecCanvas.at(cNCbc)->cd();
                    cHistVec.at(cNCbc)->Draw();
                    vecCanvas.at(cNCbc)->Close();
                }

                emit sendGraphData(vecCanvas);
                if ( cN == cNevents )
                    break;

                cNthAcq++;
            }

        }
    }


    void DataTestWorker::Run(BeBoard* pBeBoard, uint32_t pNthAcq)
    {
        m_systemController.m_SystemControllerWorker.fBeBoardInterface->Start(pBeBoard);
        m_systemController.m_SystemControllerWorker.fBeBoardInterface->ReadData(pBeBoard, pNthAcq, true );
        m_systemController.m_SystemControllerWorker.fBeBoardInterface->Stop(pBeBoard, pNthAcq);
    }

}
