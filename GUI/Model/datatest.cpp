#include "datatest.h"
#include <QDebug>
#include <QVector>
#include <QThread>
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"

#include "Model/HybridTester.h"
#include "Model/systemcontroller.h"



namespace GUI
{
    DataTest::DataTest(QObject *parent,
                       SystemController &sysController) :
        QObject(parent),
        m_systemController(sysController),
        m_thread(new QThread()),
        m_worker(new HybridTester(nullptr,
                                    sysController)),
        m_Vcth(0),
        m_Events(0)
    {
        qRegisterMetaType<std::vector<std::shared_ptr<TH1F>> >("std::vector<std::shared_ptr<TH1F>>");
        qRegisterMetaType<std::vector<std::shared_ptr<TF1>> >("std::vector<std::shared_ptr<TF1>>");
        qRegisterMetaType<std::string> ("std::string");
        qRegisterMetaType<std::map<Cbc*, TH1F*> >("std::map<Cbc*, TH1F*>");
        m_worker->moveToThread(m_thread);
        WireThreadConnections();
        WireGraphData();
    }

    DataTest::~DataTest()
    {
        m_worker->abort();
        m_thread->wait();
        delete m_thread;
        qDebug() << "Deleting DataTest worker thread " <<this->QObject::thread()->currentThreadId();
        qDebug() << "Destructing " << this;
    }

    void DataTest::WireThreadConnections()
    {
        connect(m_worker, SIGNAL(workRequested()),
                m_thread, SLOT(start()));
        connect(m_thread, SIGNAL(started()),
                m_worker, SLOT(doWork()));

        connect(m_worker, SIGNAL(finished()),
                this, SIGNAL(finishedDataTest()));

        connect(m_worker, SIGNAL(finished()),
                m_thread, SLOT(quit()), Qt::DirectConnection);
    }

    void DataTest::WireGraphData()
    {
        connect(m_worker, SIGNAL(sendOccupyHists(std::vector<std::shared_ptr<TH1F> >)),
                this, SIGNAL(sendGraphData(std::vector<std::shared_ptr<TH1F> >)), Qt::QueuedConnection);
        connect(m_worker, SIGNAL(sendHistsThreshold(std::vector<std::shared_ptr<TH1F> >,std::string)),
                this, SIGNAL(sendHistsThreshold(std::vector<std::shared_ptr<TH1F> >,std::string)), Qt::QueuedConnection);
        connect(m_worker, SIGNAL(sendFitThreshold(std::vector<std::shared_ptr<TF1> >,std::string)),
                this, SIGNAL(sendFitThreshold(std::vector<std::shared_ptr<TF1> >,std::string)), Qt::QueuedConnection);

        connect(m_worker, SIGNAL(sendHists(std::map<Cbc*,TH1F*>,std::string)),
                 this, SIGNAL(sendHists(std::map<Cbc*,TH1F*>,std::string)), Qt::QueuedConnection);
        connect(m_worker, SIGNAL(sendRefreshHists()),
                this, SIGNAL(sendRefreshHists()), Qt::QueuedConnection);
    }


    void DataTest::initialiseSettings()
    {
        emit getVcthValue();
        emit getEventsValue();
        emit getIsRegTestChecked();
        emit getIsScanChecked();
        emit getIsHoleModeChecked();

        emit startedDataTest();

        m_worker->abort();
        m_thread->wait();

        m_worker->requestWork(m_Vcth, m_Events, m_TestReg, m_ScanThreshold, m_HoleMode);
    }

    void DataTest::TestFinish()
    {
        m_worker->abort();
        m_thread->deleteLater();
        //m_thread->deleteLater();
    }

    void DataTest::setVcthValue(int cVcth)
    {
        m_Vcth = cVcth;
    }

    void DataTest::setEventsValue(int cEvents)
    {
        m_Events = cEvents;
    }

    void DataTest::setTestReg(const bool testReg)
    {
        m_TestReg = testReg;
    }

    void DataTest::setScanThreshold(const bool scanThreshhold)
    {
        m_ScanThreshold = scanThreshhold;
    }

    void DataTest::setHoleMode(const bool holeMode)
    {
        m_HoleMode = holeMode;
    }

}
