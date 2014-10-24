#include "datatestworker.h"
#include <QDebug>
#include <QThread>
#include <vector>

#include <TH1.h>
#include "TCanvas.h"
#include "utils.h"

#include "../tools/HybridTester.h"


#include "unistd.h"


using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;


namespace GUI
{
    DataTestWorker::DataTestWorker(QObject *parent,
                                   SystemController &sysController) ://, SystemController &sysCtrl) :
        QObject(parent),
        m_systemController(sysController),
        _Drawing(true) // stops refresh being called
    {
    }

    DataTestWorker::~DataTestWorker()
    {
        qDebug() << "Destructing " << this;
    }

    void DataTestWorker::requestWork(int cVcth, int cEvents, const std::vector<TCanvas *> canvas)
    {
        m_Vcth = cVcth;
        m_Events = cEvents;
        m_canvas = canvas;

        /*Mutex.lock();
        _Drawing = false;
        this->Mutex.unlock();*/

        qDebug()<<"Request worker start in Thread "<<thread()->currentThreadId();

        emit workRequested();
    }

    void DataTestWorker::abort()
    {
        //TODO
        /*mutex.lock();
        if (_working) {
            _abort = true;
            qDebug()<<"Request worker aborting in Thread "<<thread()->currentThreadId();
        }
        mutex.unlock();*/
    }

    void DataTestWorker::doWork()
    {
        /*std::vector<TH1D*> hist;

        int no = 0;
        for (auto& cCanvas : m_canvas)
        {
            TString name("Data Test Cbc ");
            name.Append(std::to_string(no));
            hist.push_back(new TH1D (name.Data(),name.Data(), 500, 0, 500));
            no++;
        }
        int total = 0;

        int i = 0;
        for (int j= 0; j<200; j++){
            int n = 0;
            for (auto& cCanvas : m_canvas)
            {
                ++total;
                qDebug() << total;
                usleep(100);

                if(this->Mutex.try_lock_for(std::chrono::milliseconds(100)))
                {
                    qDebug() << "Locking in thread";
                    _Drawing = true;
                    this->Mutex.unlock();

                    qDebug() << "Draw is " << _Drawing;

                    hist.at(n)->Fill(j+n);
                    cCanvas->cd();
                    hist.at(n)->Draw();

                    this->Mutex.lock();
                    qDebug() << "Thread Locked";
                    _Drawing = false;
                    this->Mutex.unlock();
                    qDebug() << "Thread Unlocked";

                    qDebug() << "Draw is " << _Drawing;

                    i++;
                    n++;
                }
                else
                {
                    qDebug() << "!!!!!!!!!!Trying to fill graph timed out for CBC " << n;
                }
            }

        }*/


        std::string cHWFile = ( "settings/Calibration2CBC.xml");
        std::string cDirectory = ("Results/");
        bool cScan=false;

        HybridTester cHybridTester;
        qDebug() << "Started";
        cHybridTester.InitializeHw( cHWFile );
        qDebug() << "1";
        cHybridTester.InitializeHistsGUI(cScan, m_canvas);
        qDebug() << "2";
        cHybridTester.InitializeSettings( cHWFile );
        qDebug() << "3";
        cHybridTester.CreateResultDirectory( cDirectory );
        qDebug() << "4";
        cHybridTester.InitResultFile( "HybridTest" );
        qDebug() << "5";
        cHybridTester.ConfigureHw();

        emit finished();

    }


    void DataTestWorker::Run( BeBoard* pBeBoard, uint32_t pNthAcq )
    {
        fBeBoardInterface->Start( pBeBoard );
        fBeBoardInterface->ReadData( pBeBoard, pNthAcq, true );
        fBeBoardInterface->Stop( pBeBoard, pNthAcq );
    }

}
