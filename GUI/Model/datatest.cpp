#include "datatest.h"

#include "Model/systemcontroller.h"

#include "lib/qcustomplot.h"
#include <QVector>
#include <QThread>

#include "datatestworker.h"



namespace GUI
{
    DataTest::DataTest(QObject *parent,
                       SystemController &sysCtrl) :
        QObject(parent),
        m_systemController(sysCtrl),
        m_thread(new QThread()),
        m_worker(new DataTestWorker)
    {
        //qRegisterMetaType<QVector<double> >();
        qRegisterMetaType<QVector<double> >("QVector<double>");
        m_worker->moveToThread(m_thread);
        WireThreadConnections();
    }

    DataTest::~DataTest()
    {
        m_worker->abort();
        m_thread->wait();
        delete m_thread;
        qDebug() << "Deleting DataTest worker thread " <<this->QObject::thread()->currentThreadId();
        //wait();
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


        connect(m_worker, SIGNAL(sendGraphData(QVector<double>,QVector<double>)),
                this, SLOT(relaySendGraphData(QVector<double>,QVector<double>)));
    }

    void DataTest::createGraph()
    {

        m_worker->abort();
        m_thread->wait(); // If the thread is not running, this will immediately return.

        m_worker->requestWork();

    }

    void DataTest::relaySendGraphData(const QVector<double> &valueX, const QVector<double> &valueY)
    {
        //qDebug() << valueX;
        emit sendGraphData(valueX, valueY);
    }

}
