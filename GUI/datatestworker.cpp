#include "datatestworker.h"

#include "Model/systemcontroller.h"
#include <QDebug>

#include <QVector>
#include <QThread>

#include <QEventLoop>
#include <QTimer>

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


namespace GUI
{
    DataTestWorker::DataTestWorker(QObject *parent) :
        QObject(parent)
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

        /*if (abort) {
            qDebug()<<"Aborting worker process in Thread "<<thread()->currentThreadId();
            break;
        }*/

        ReadDataTest();

        // Set _working to false, meaning the process can't be aborted anymore.
        mutex.lock();
        _working = false;
        mutex.unlock();

        qDebug()<<"Worker process finished in Thread "<<thread()->currentThreadId();

        //Once 60 sec passed, the finished signal is sent
        emit finished();
    }

    void DataTestWorker::ReadDataTest()
    {
        // Checks if the process should be aborted
        //mutex.lock();
        //bool abort = _abort;
        //mutex.unlock();

        //SystemController cSystemController;

        QVector<double> x ;//= new QVector<double>;
        QVector<double> y ;//= new QVector<double>;



        for (int i = 0; i < 60; i ++) {

            // Checks if the process should be aborted
            mutex.lock();
            bool abort = _abort;
            mutex.unlock();

            if (abort) {
                qDebug()<<"Aborting worker process in Thread "<<thread()->currentThreadId();
                break;
            }

            // This will stupidly wait 1 sec doing nothing...
            QEventLoop loop;
            QTimer::singleShot(1000, &loop, SLOT(quit()));
            loop.exec();

            x.push_back(i);
            y.push_back(i);

            // Once we're done waiting, value is updated
            //emit valueChanged(QString::number(i));

             emit sendGraphData(x, y);
        }



        //emit sendGraphData(x, y);





        /*std::vector<TH1F*> cHistVec;
        for ( uint8_t cNCbc = 0; cNCbc < cSystemController.fShelveVec[0]->getBoard( 0 )->getModule( 0 )->getNCbc(); cNCbc++ )
            cHistVec.push_back( new TH1F( Form( "Histo_Hits_CBC%d", cNCbc ), Form( "Occupancy_CBC%d", cNCbc ), 255, -0.5, 254.5 ) );

        uint32_t cNthAcq = 0;

        for ( uint32_t cVCth = 100; cVCth < 140; cVCth += 2 )
        {
            cSystemController.fCbcInterface->WriteCbcReg( cSystemController.fShelveVec[0]->getBoard( 0 )->getModule( 0 )->getCbc( 0 ), "VCth", cVCth );
            cSystemController.fCbcInterface->WriteCbcReg( cSystemController.fShelveVec[0]->getBoard( 0 )->getModule( 0 )->getCbc( 1 ), "VCth", cVCth );

            uint32_t cNevents = 200;
            uint32_t cN = 0;

            for ( uint8_t cNCbc = 0; cNCbc < cSystemController.fShelveVec[0]->getBoard( 0 )->getModule( 0 )->getNCbc(); cNCbc++ )
            {
                delete cHistVec[cNCbc];
                cHistVec[cNCbc] = new TH1F( Form( "Histo_Hits_CBC%d", cNCbc ), Form( "Occupancy_CBC%d", cNCbc ), 255, -0.5, 254.5 );
            }

            gStyle->SetOptStat( 0 );

            usleep( 100 );

            while ( !( cN == cNevents ) )
            {

                cSystemController.Run( cSystemController.fShelveVec[0]->getBoard( 0 ), cNthAcq );

                const Event* cEvent = cSystemController.fBeBoardInterface->GetNextEvent( cSystemController.fShelveVec[0]->getBoard( 0 ) );

                while ( cEvent )
                {

                    if ( cNevents != 0 && cN == cNevents )
                        break;

                    for ( uint8_t cNFe = 0; cNFe < cSystemController.fShelveVec[0]->getBoard( 0 )->getNFe(); cNFe++ )
                    {

                        for ( uint8_t cNCbc = 0; cNCbc < cSystemController.fShelveVec[0]->getBoard( 0 )->getModule( cNFe )->getNCbc(); cNCbc++ )
                        {
                            uint32_t cNHits = 0;
                            std::vector<bool> cDataBitVector = cEvent->DataBitVector( cNFe, cNCbc );

                            for ( uint8_t cDBVec = 0; cDBVec < cDataBitVector.size(); cDBVec++ )
                            {
                                if ( cDataBitVector[cDBVec] )
                                    cNHits++;
                            }
                            cHistVec[cNCbc]->Fill( cNHits );
                        }
                    }

                    cEvent = cSystemController.fBeBoardInterface->GetNextEvent( cSystemController.fShelveVec[0]->getBoard( 0 ) );
                    cN++;
                }

                for ( uint8_t cNCbc = 0; cNCbc < cHistVec.size(); cNCbc++ )
                {
                    cCanvas->cd( uint32_t( cNCbc ) + 1 );
                    cHistVec[cNCbc]->Draw();
                }

                cCanvas->Update();

                if ( cN == cNevents )
                    break;

                cNthAcq++;

            }

            cCanvas->Print( ( ( boost::format( "output/Histogram_Vcth_%d.pdf" ) % ( cVCth ) ).str() ).c_str() );

        }*/


    }

}
