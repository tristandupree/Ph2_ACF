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
        std::string cHWFile = ( "settings/Calibration2CBC.xml");
        std::string cDirectory = ("Results/");
        bool cScan=false;

        HybridTester cHybridTester;
        cHybridTester.InitializeHw( cHWFile );
        //cHybridTester.InitializeHists( cScan );
        cHybridTester.InitializeHistsGUI(cScan, m_canvas);
        cHybridTester.InitializeSettings( cHWFile );
        cHybridTester.CreateResultDirectory( cDirectory );
        cHybridTester.InitResultFile( "HybridTest" );
        cHybridTester.ConfigureHw();



    }


    void DataTestWorker::Run( BeBoard* pBeBoard, uint32_t pNthAcq )
    {
        fBeBoardInterface->Start( pBeBoard );
        fBeBoardInterface->ReadData( pBeBoard, pNthAcq, true );
        fBeBoardInterface->Stop( pBeBoard, pNthAcq );
    }

}
