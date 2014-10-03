#include "datatestworker.h"
#include "Model/systemcontrollersettings.h"
#include <QDebug>
#include <QVector>
#include <QThread>

#include <QEventLoop>
#include <QTimer>

#include <boost/format.hpp>

#include <TH1F.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TApplication.h>
#include <TROOT.h>


using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;


namespace GUI
{
    DataTestWorker::DataTestWorker(QObject *parent,
                                   SystemControllerSettings &sysSettings) ://, SystemController &sysCtrl) :
        QObject(parent),
        m_systemSettings(sysSettings)
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
        qDebug()<<"Starting worker process in Thread "<<thread()->currentThreadId();

        ReadDataTest();

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

        QVariantMap mapGraphs;
        uint32_t cNthAcq = 0;
        //push back a container of values




        for (uint32_t cVCth = 100; cVCth<140; cVCth+=2)
        {
            if (abort)
            {
                qDebug()<<"Aborting worker process in Thread "<<thread()->currentThreadId();
                break;
            }
            for ( uint8_t cNCbc = 0; cNCbc < m_systemSettings.fShelveVec[0]->getBoard( 0 )->getModule( 0 )->getNCbc(); cNCbc++ )
            {
                m_systemSettings.fCbcInterface->WriteCbcReg( m_systemSettings.fShelveVec[0]->getBoard( 0 )->getModule( 0 )->getCbc(cNCbc), "VCth", cVCth );
            }
            uint32_t cNevents = 200;
            uint32_t cN = 0;

            while ( !( cN == cNevents ) )
            {
                Run(m_systemSettings.fShelveVec[0]->getBoard(0), cNthAcq);
                const Event* cEvent = m_systemSettings.fBeBoardInterface->GetNextEvent( m_systemSettings.fShelveVec[0]->getBoard( 0 ) );
                while ( cEvent )
                {
                    if ( cNevents != 0 && cN == cNevents )
                        break;

                    for ( uint8_t cNFe = 0; cNFe < m_systemSettings.fShelveVec[0]->getBoard( 0 )->getNFe(); cNFe++ )
                    {
                        for ( uint8_t cNCbc = 0; cNCbc < m_systemSettings.fShelveVec[0]->getBoard( 0 )->getModule( 0 )->getNCbc(); cNCbc++ )
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

                            if (!cNHits==0)
                            {
                                QString qCbc = QString::number(cNCbc,16);
                                mapGraphs.insert(qCbc, cNHits);
                                emit sendGraphData(mapGraphs);
                            }
                        }
                    }

                    cEvent = m_systemSettings.fBeBoardInterface->GetNextEvent( m_systemSettings.fShelveVec[0]->getBoard( 0 ) );
                    cN++;
                }
            }
        }
    }


    void DataTestWorker::Run(BeBoard* pBeBoard, uint32_t pNthAcq)
    {
        m_systemSettings.fBeBoardInterface->Start(pBeBoard);
        m_systemSettings.fBeBoardInterface->ReadData(pBeBoard, pNthAcq, true );
        m_systemSettings.fBeBoardInterface->Stop(pBeBoard, pNthAcq);
    }

}
