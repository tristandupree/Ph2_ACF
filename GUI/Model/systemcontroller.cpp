#include "Model/systemcontroller.h"
#include "settings.h"
#include "Model/systemcontrollerworker.h"

#include <QThread>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string.h>

#include"provider.h"

#include<QDebug>

#include <QVariantMap>

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;


namespace GUI
{
    SystemController::SystemController(QObject *parent,
                                       Settings &config) :
        QObject(parent),
        m_Settings(config),
        m_thread(new QThread()),
        m_worker(new SystemControllerWorker(nullptr,
                                            config))
    {
        m_worker->moveToThread(m_thread);
        WireThreadConnections();
    }

    SystemController::~SystemController()
    {
        m_worker->abort();
        //m_thread->wait();
        delete m_thread;
        qDebug() << "Deleting DataTest worker thread " <<this->QObject::thread()->currentThreadId();
        qDebug() << "Destructing " << this;
    }

    void SystemController::WireThreadConnections()
    {
        connect(m_worker, SIGNAL(workRequested()),
                m_thread, SLOT(start()));

        connect(m_thread, SIGNAL(started()),
                m_worker, SLOT(InitializeHw()));

        connect(m_worker, SIGNAL(workConfigureHwRequested()),
                m_worker, SLOT(ConfigureHw()));

        connect(m_worker, SIGNAL(finishedInitialiseHw()),
                this, SLOT(finishInitialiseHw()));
        connect(m_worker, SIGNAL(finishedConfigureHw()),
                this, SLOT(finishConfigureHw()));

        /*connect(m_worker, SIGNAL(finished()),
                m_thread, SLOT(quit()), Qt::DirectConnection);*/

        /*connect(m_worker, SIGNAL(sendGraphData(std::vector<TCanvas*>)),
                this, SLOT(relaySendGraphData(std::vector<TCanvas*>)),
                Qt::QueuedConnection);*/
    }

    void SystemController::startInitialiseHw()
    {
        m_worker->requestWork();
    }

    void SystemController::startConfigureHw()
    {
        m_worker->requestConfigureHw();
        //m_worker->ConfigureHw2();
    }

    void SystemController::finishInitialiseHw()
    {
        SendStatusMessage(tr("Initialised Hw"));
        emit notifyInitFinished();
    }

    void SystemController::finishConfigureHw()
    {
        SendStatusMessage(tr("Hardware configured"));
        emit notifyConfigFinished();
    }

    void SystemController::SendStatusMessage(QString msg)
    {
        emit notifyStatusMessage(msg);
    }
}


