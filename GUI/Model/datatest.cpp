#include "datatest.h"
#include <QDebug>
#include <QVector>
#include <QThread>
#include "TH1D.h"
#include "TCanvas.h"


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
        qRegisterMetaType<std::vector<std::shared_ptr<TH1D>> >("std::vector<std::shared_ptr<TH1D>>");
        qRegisterMetaType<TCanvas*>("TCanvas*");
        qRegisterMetaType<std::vector<TCanvas*> >("std::vector<TCanvas*>");
        m_worker->moveToThread(m_thread);
        WireThreadConnections();
        //connect()
        connect(m_timer, SIGNAL(timeout()),
                this, SIGNAL(sendRefresh()));
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

        connect(m_worker, SIGNAL(sendGraphData(std::vector<std::shared_ptr<TH1D>>)),
                this, SIGNAL(sendGraphData(std::vector<std::shared_ptr<TH1D>>)), Qt::QueuedConnection);
    }

    void DataTest::createGraph()
    {
        emit getVcthValue();
        emit getEventsValue();
        emit startedDataTest();
        qDebug()<<"Entered Create Graph";
        emit getTCanvas();


        //m_worker->abort();
        //m_thread->wait(); // If the thread is not running, this will immediately return.

        // m_worker->requestWork(m_Vcth, m_Events, );

    }



    void DataTest::setVcthValue(int cVcth)
    {
        m_Vcth = cVcth;
    }

    void DataTest::setEventsValue(int cEvents)
    {
        m_Events = cEvents;
    }

    void DataTest::recieveTCanvas(std::vector<TCanvas *> canvas)
    {
        qDebug() << "Size of canvas" << canvas.size();

        m_worker->requestWork(m_Vcth, m_Events, canvas);
        m_timer->start(100);
        //m_timer->start(100);
        //m_timer->stop();

        //m_worker->abort();
        //m_thread->wait();

        /* for (int i; i < canvas.size(); i++)
        {
            canvas.at(i)->SetFillColor(i+5);

            TH1D* g = new TH1D("hjsh", "jhea", 10, 0, 10);
            canvas.at(i)->cd();
            g->Draw();

            qDebug() << canvas.at(i)->GetName();

            qDebug() << "DrawnGraph";
            emit sendRefresh();
            emit finishedDataTest();
            //emit
        }*/
    }

}
