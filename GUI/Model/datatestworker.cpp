#include "datatestworker.h"
#include <QDebug>
#include <QThread>
#include <vector>

#include <TH1.h>
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

    void DataTestWorker::requestWork(int cVcth, int cEvents)
    {
        m_Vcth = cVcth;
        m_Events = cEvents;

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
        /*static Int_t HistoID = 1;


        std::vector<TH1D*> graphs;
        TString name("h1_");

        for (int i = 0; i <2 ; i++)
        {
            name += HistoID++;

            TH1D *h1 = new TH1D(name.Data(),name.Data(),1000,0, 2000);
            graphs.push_back(h1);
        }

        for (int t = 0; t < 400; t++)
        {
            usleep(10000);
            for (int i = 0; i <2 ; i++)
            {
                graphs.at(i)->Fill(t*2);
                emit sendGraphData(graphs);
            }
        }
        emit sendGraphData(graphs);*/

        /*if (m_vecGraphs.size()!=0)
        {
            for (auto& kv : m_vecGraphs)
            {
                delete kv;
            }

            qDebug()<< "Size now " << m_vecGraphs.size();
        }*/

        ReadDataTest();

        // Set _working to false, meaning the process can't be aborted anymore.
        mutex.lock();
        _working = false;
        mutex.unlock();

        qDebug()<<"Worker process finished in DataTest thread "<<thread()->currentThreadId();

        emit finished();
    }



    void DataTestWorker::ReadDataTest()
    {
        // Checks if the process should be aborted
        mutex.lock();
        bool abort = _abort;
        mutex.unlock();



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

                        TH1D *h1 = new TH1D(name.Data(),name.Data(),254,0, 254);

                        //cHistSVec.push_back(std::make_shared<TH1D>(new TH1D(name.Data(),name.Data(),254,0, 254)));
                        auto h = std::make_shared<TH1D>(name.Data(),name.Data(), 254, 0, 254);
                        cHistSVec.push_back(h);

                        m_vecGraphs.push_back(h1);
                        fCbcInterface->WriteCbcReg( &cCbc, "VCth", uint8_t( m_Vcth ) );

                    }
                }
            }
        }

        uint32_t cN = 0;
        uint32_t cNthAcq = 0;

        while ( cN < pEventsperVcth )
        {
            BeBoard pBoard = fShelveVector.at( 0 )->fBoardVector.at( 0 );
            Run( &pBoard, cNthAcq );

            const Event* cEvent = fBeBoardInterface->GetNextEvent( &pBoard );

            while ( cEvent )
            {
                std::cout << " cVcth = " << uint32_t( m_Vcth ) << std::endl;
                std::cout << ">>> Event #" << cN << std::endl;
                std::cout << *cEvent << std::endl;
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

                                for ( uint8_t cDBVec = 0; cDBVec < cVecData.size(); cDBVec++ )
                                {
                                    if ( cVecData[cDBVec] )
                                        cNHits++;
                                }
                                if(cNHits!=0)
                                {
                                    m_vecGraphs.at(cNCbc)->Fill(cNHits);
                                    qDebug() << cNHits;
                                }

                                cNCbc ++;
                            }
                            emit sendGraphData(m_vecGraphs);
                        }
                    }
                }
                if ( cN == pEventsperVcth )
                    break;
                cN++;
                //emit sendGraphData(graphs);

                if ( cN < pEventsperVcth )
                    cEvent = fBeBoardInterface->GetNextEvent( &pBoard );
                else break;
            }
            cNthAcq++;

            qDebug()<<"Worker process finished in DataTest thread "<<thread()->currentThreadId();

            emit finished();
        }
    }

    void DataTestWorker::Run( BeBoard* pBeBoard, uint32_t pNthAcq )
    {
        fBeBoardInterface->Start( pBeBoard );
        fBeBoardInterface->ReadData( pBeBoard, pNthAcq, true );
        fBeBoardInterface->Stop( pBeBoard, pNthAcq );
    }

}
