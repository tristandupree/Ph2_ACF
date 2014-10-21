#include "datatestworker.h"
#include <QDebug>
#include <QThread>
#include <vector>
#include <QTimer>

#include <TH1.h>
#include "TCanvas.h"
#include "utils.h"


#include "unistd.h"


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

    void DataTestWorker::requestWork(int cVcth, int cEvents, std::vector<TCanvas *> canvas)
    {
        m_Vcth = cVcth;
        m_Events = cEvents;
        m_canvas = canvas;

        /*for (auto& cCanvas : m_canvas)
        {
            cCanvas->SetFillColor(KRED);

            TH1D* g = new TH1D("hjsh", "jhea", 10, 0, 10);
            cCanvas->cd();
            g->Draw();
        }*/

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
        //ReadDataTest();

        std::vector<std::shared_ptr<TH1D>> vecHist;
       // std::vector<TH1D*> = hist;

        int no = 0;
        for (auto& cCanvas : m_canvas)
        {
            TString name("Data Test Cbc ");
            name.Append(std::to_string(no));
            auto h = std::make_shared<TH1D>(name.Data(),name.Data(), 250, 0, 250);
            name.Data(),name.Data(), 250, 0, 250;
            vecHist.push_back(h);
            no++;
        }

        int i = 0;
        for (int j= 0; j<5; j++){
            int n = 0;
            for (auto& cCanvas : m_canvas)
            {
                usleep(100000);
                vecHist.at(n)->Fill(j+n);
                usleep(100000);
                cCanvas->cd();
                vecHist.at(n)->Draw();
                //cCanvas->SetFillColor(i+3);

                //TH1D* g = new TH1D("hjsh", "jhea", 10, 0, 10);


                //cCanvas->cd();
                //g->Draw();
                i++;
                n++;
            }
        }
        mutex.lock();
        _working = false;
        mutex.unlock();

        qDebug()<<"Worker process finished in DataTest thread "<<thread()->currentThreadId();
        //emit finished();
    }



    void DataTestWorker::ReadDataTest()
    {
        mutex.lock();
        bool abort = _abort;
        mutex.unlock();

        std::vector<std::shared_ptr<TH1D>> vecHist;
        std::vector<TH1D*> vecTH1D;

        int pEventsperVcth = m_Events;

        fBeBoardInterface = m_systemController.getBeBoardInterface();
        fCbcInterface = m_systemController.getCbcInterface();
        fShelveVector = m_systemController.getfShelveVector();
        fBeBoardFWMap = m_systemController.getBeBoardFWMap();


        for ( auto cShelve : fShelveVector )
        {
            for ( auto cBoard : ( cShelve )->fBoardVector )
            {
                for ( auto cFe : cBoard.fModuleVector )
                {
                    for ( auto cCbc : cFe.fCbcVector )
                    {
                        TString name("Data Test Cbc ");
                        name.Append(std::to_string(cCbc.getCbcId()));
                        auto h = std::make_shared<TH1D>(name.Data(),name.Data(), 250, 0, 250);
                        vecHist.push_back(h);

                        TH1D* g = new TH1D("hjsh", "jhea", 260, 0, 260);
                        vecTH1D.push_back(g);

                        fCbcInterface->WriteCbcReg( &cCbc, "VCth", uint8_t( m_Vcth ) );
                    }
                }
            }
        }

        uint32_t cN = 0;
        uint32_t cNthAcq = 0;

        while ( cN < pEventsperVcth )
        {
            if(cN == pEventsperVcth) break;
            BeBoard pBoard = fShelveVector.at( 0 )->fBoardVector.at( 0 );
            Run( &pBoard, cNthAcq );

            const Event* cEvent = fBeBoardInterface->GetNextEvent( &pBoard );

            while ( cEvent )
            {
                std::cout << " cVcth = " << uint32_t( m_Vcth ) << std::endl;
                std::cout << ">>> Event #" << cN << std::endl;

                for ( auto cShelve : fShelveVector )
                {
                    for ( auto cBoard : ( cShelve )->fBoardVector )
                    {
                        for ( auto cFe : cBoard.fModuleVector )
                        {
                            uint8_t cNCbc = 0;
                            for ( auto cCbc : cFe.fCbcVector )
                            {
                                uint32_t cNHits = 0;
                                std::vector<bool> cVecData = cEvent->DataBitVector(cFe.getFeId(), cCbc.getCbcId());

                                for ( uint8_t cDBVec = 0; cDBVec < vecHist.size(); cDBVec++ )
                                {
                                    if ( cVecData[cDBVec] )
                                        cNHits++;
                                }
                                //vecHist.at(cNCbc)->Fill(cNHits);
                                //vecHist.at(cNCbc)->Draw();

                                qDebug() << cNHits;

                                cNCbc ++;
                            }
                        }
                    }
                }

                if ( cN == pEventsperVcth )
                    break;
                cN++;

                if ( cN < pEventsperVcth )
                    cEvent = fBeBoardInterface->GetNextEvent( &pBoard );
                else break;
            }
            cNthAcq++;
        }
        cNthAcq++;
    }


    void DataTestWorker::Run( BeBoard* pBeBoard, uint32_t pNthAcq )
    {
        fBeBoardInterface->Start( pBeBoard );
        fBeBoardInterface->ReadData( pBeBoard, pNthAcq, true );
        fBeBoardInterface->Stop( pBeBoard, pNthAcq );
    }

}
