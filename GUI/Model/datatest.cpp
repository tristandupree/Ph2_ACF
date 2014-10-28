#include "datatest.h"
#include <QDebug>
#include <QVector>
#include <QThread>
#include "TH1D.h"
#include "TCanvas.h"
#include <atomic>


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
        m_timer(new QTimer(this)),
        m_Vcth(0),
        m_Events(0)
    {
        qRegisterMetaType<std::vector<TCanvas*> >("std::vector<TCanvas*>");
        m_worker->moveToThread(m_thread);
        WireThreadConnections();
        WireTimer();
    }

    DataTest::~DataTest()
    {
        m_worker->abort();
        m_thread->wait();
        delete m_thread;
        qDebug() << "Deleting DataTest worker thread " <<this->QObject::thread()->currentThreadId();
        m_timer->stop();
        qDebug() << "Deleting timer";
        delete m_timer;
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
           this, SIGNAL(sendRefresh()));
        connect(m_worker, SIGNAL(finished()),
                m_thread, SLOT(quit()), Qt::DirectConnection);
    }

    void DataTest::WireTimer()
    {
        connect(m_timer, SIGNAL(timeout()),
                this, SLOT(tryRefresh()), Qt::QueuedConnection);
        connect(m_worker, SIGNAL(finished()),
                m_timer, SLOT(stop()));
    }

    void DataTest::createGraph()
    {
        emit getVcthValue();
        emit getEventsValue();
        emit startedDataTest();
        emit getTCanvas();
    }

    void DataTest::setVcthValue(int cVcth)
    {
        m_Vcth = cVcth;
    }

    void DataTest::setEventsValue(int cEvents)
    {
        m_Events = cEvents;
    }

    void DataTest::recieveTCanvas(const std::vector<TCanvas *> canvas)
    {
        qDebug() << "Size of canvas" << canvas.size();
        canvas.at(0);
        canvas.at(0)->SetName( "fDataCanvas" );
        canvas.at(0)->SetTitle( "SingleStripEfficiency" );
        //canvas.at(0)->Divide( 2 );

        emit sendRefresh();


        m_worker->abort();
        m_thread->wait();
        m_timer->start(100);
        m_worker->requestWork(m_Vcth, m_Events, canvas);
    }

    void DataTest::tryRefresh()
    {
        /*if (!m_worker->_Drawing)
        {
            qDebug()<< ">> get is " << m_worker->_Drawing;
            for (int i = 0; i < 10; i++)
            {
                qDebug() << " >> Attempting lock in worker";
                if(m_worker->Mutex.try_lock_for(std::chrono::milliseconds(100)))
                {
                    if (!m_worker->_Drawing)
                    {
                        qDebug() << " >> Worker Locked";
                        emit sendRefresh();
                        std::chrono::microseconds(100);
                        m_worker->Mutex.unlock();
                        qDebug() << " >> Worker Unlocked";
                        usleep(10);
                    }
                }
                else
                {
                    qDebug() << "Unable to lock Mutex!!";
                }
            }
        }*/
    }



}
