#include "datatest.h"

#include "Model/systemcontrollersettings.h"
#include <QDebug>
#include <QVector>
#include <QThread>
#include "TCanvas.h"

#include "datatestworker.h"



namespace GUI
{
    DataTest::DataTest(QObject *parent,
                       SystemControllerSettings &sysCtrlSettings) :
        QObject(parent),
        m_systemControllerSettings(sysCtrlSettings),
        m_thread(new QThread()),
        m_worker(new DataTestWorker(nullptr, sysCtrlSettings))
    {
        qRegisterMetaType<std::vector<TCanvas*> >("std::vector<TCanvas*>");
        m_worker->moveToThread(m_thread);
        WireThreadConnections();
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
                m_thread, SLOT(quit()), Qt::DirectConnection);


        connect(m_worker, SIGNAL(sendGraphData(std::vector<TCanvas*>)),
                this, SLOT(relaySendGraphData(std::vector<TCanvas*>)),
                Qt::QueuedConnection);
    }

    void DataTest::createGraph()
    {

        m_worker->abort();
        m_thread->wait(); // If the thread is not running, this will immediately return.

        m_worker->requestWork();

    }

    void DataTest::relaySendGraphData(const std::vector<TCanvas*> &canvas)
    {
        emit sendGraphData(canvas);
    }

}
