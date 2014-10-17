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
        
        // Set _working to false, meaning the process can't be aborted anymore.
        mutex.lock();
        _working = false;
        mutex.unlock();

        qDebug()<<"Worker process finished in DataTest thread "<<thread()->currentThreadId();

        //emit finished();
    }



    void DataTestWorker::ReadDataTest()
    {
        // Checks if the process should be aborted
        mutex.lock();
        bool abort = _abort;
        mutex.unlock();

        std::vector<std::shared_ptr<TH1D>> m_vecHist;

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
                        auto h = std::make_shared<TH1D>(name.Data(),name.Data(), 50, 0, 50);
                        m_vecHist.push_back(h);
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
                //std::cout << *cEvent << std::endl;

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

                                for ( uint8_t cDBVec = 0; cDBVec < m_vecHist.size(); cDBVec++ )
                                {
                                    if ( cVecData[cDBVec] )
                                        cNHits++;
                                }
                                m_vecHist.at(cNCbc)->Fill(cNHits);
                                qDebug() << cNHits;

                                cNCbc ++;
                            }
                            emit sendGraphData(m_vecHist);
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

        qDebug()<<"Worker process finished in DataTest thread "<<thread()->currentThreadId();

        emit finished();
    }


    void DataTestWorker::Run( BeBoard* pBeBoard, uint32_t pNthAcq )
    {
        fBeBoardInterface->Start( pBeBoard );
        fBeBoardInterface->ReadData( pBeBoard, pNthAcq, true );
        fBeBoardInterface->Stop( pBeBoard, pNthAcq );
    }

}
