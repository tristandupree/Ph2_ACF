#include "cmtest.h"
#include <QDebug>
#include <QThread>

#include "Model/cmtestworker.h"

namespace GUI{
    CmTest::CmTest(QObject *parent) :
        QObject(parent),
        m_thread(new QThread()),
        m_worker(new CmTestWorker(nullptr))
    {
        m_worker->moveToThread(m_thread);
        WireThreadConnections();
    }

    CmTest::~CmTest()
    {
        m_thread->wait();
        delete m_thread;
        qDebug() << "Deleting CmTest worker thread " <<this->QObject::thread()->currentThreadId();
        qDebug() << "Destructing " << this;
    }

    void CmTest::WireThreadConnections()
    {

    }
}
