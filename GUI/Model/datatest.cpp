#include "datatest.h"
#include <QDebug>
#include <QVector>
#include <QThread>
#include "TH1D.h"

#include "Model/datatestworker.h"
#include "Model/systemcontroller.h"



namespace GUI
{
    DataTest::DataTest(QObject *parent,
                       SystemController &sysController) :
        QObject(parent),
        m_systemController(sysController),
        m_thread(new QThread()),
        m_worker(new DataTestWorker(nullptr,
                                    sysController)),
        m_Vcth(0),
        m_Events(0)
    {
        qRegisterMetaType<std::vector<TH1D*> >("std::vector<TH1D*>");
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


        connect(m_worker, SIGNAL(sendGraphData(std::vector<TH1D*>)),
                this, SLOT(relaySendGraphData(std::vector<TH1D*>)),
                Qt::QueuedConnection);
    }

    void DataTest::createGraph()
    {
        emit getVcthValue();
        emit getEventsValue();

        m_worker->abort();
        m_thread->wait(); // If the thread is not running, this will immediately return.

        m_worker->requestWork(m_Vcth, m_Events);

    }

    void DataTest::relaySendGraphData(const std::vector<TH1D*> graphs)
    {
        emit sendGraphData(graphs);
    }

    void DataTest::setVcthValue(int cVcth)
    {
        m_Vcth = cVcth;
    }

    void DataTest::setEventsValue(int cEvents)
    {
        m_Events = cEvents;
    }

}
