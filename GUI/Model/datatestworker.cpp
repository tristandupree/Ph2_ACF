#include "datatestworker.h"
#include "Model/systemcontrollersettings.h"
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

        /*cHistVec.push_back(new TH1F("Histo","HistoName",10,0, 10));
        cHistVec[0]->Fill(1);
        emit sendGraphData(cHistVec);*/





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


        for ( uint8_t cNCbc = 0; cNCbc < m_systemSettings.fShelveVec[0]->getBoard( 0 )->getModule( 0 )->getNCbc(); cNCbc++ )
            cHistVec.push_back( new TH1F( Form( "Histo_Hits_CBC%d", cNCbc ), Form( "Occupancy_CBC%d", cNCbc ), 255, -0.5, 254.5 ) );

        uint32_t cNthAcq = 0;
        //uint32_t cVCth = 100;

        for (uint32_t cVCth = 100; cVCth<140; cVCth+=2)
        {

            m_systemSettings.fCbcInterface->WriteCbcReg( m_systemSettings.fShelveVec[0]->getBoard( 0 )->getModule( 0 )->getCbc( 0 ), "VCth", cVCth );
            m_systemSettings.fCbcInterface->WriteCbcReg( m_systemSettings.fShelveVec[0]->getBoard( 0 )->getModule( 0 )->getCbc( 1 ), "VCth", cVCth );
            if (abort)
            {
                qDebug()<<"Aborting worker process in Thread "<<thread()->currentThreadId();
                break;
            }

            for ( uint8_t cNCbc = 0; cNCbc < m_systemSettings.fShelveVec[0]->getBoard( 0 )->getModule( 0 )->getNCbc(); cNCbc++ )
            {
                delete cHistVec[cNCbc];
                cHistVec[cNCbc] = new TH1F( Form( "Histo_Hits_CBC%d", cNCbc ), Form( "Occupancy_CBC%d", cNCbc ), 255, -0.5, 254.5 );
            }

           // m_systemSettings.fCbcInterface->WriteCbcReg( m_systemSettings.fShelveVec[0]->getBoard( 0 )->getModule( 0 )->getCbc(cNCbc), "VCth", cVCth );

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

                            cHistVec[cNCbc]->Fill( cNHits );

                        }
                    }

                    cEvent = m_systemSettings.fBeBoardInterface->GetNextEvent( m_systemSettings.fShelveVec[0]->getBoard( 0 ) );
                    cN++;
                }

                emit sendGraphData(cHistVec);
                if ( cN == cNevents )
                    break;

                cNthAcq++;
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
