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
#include <QVector>

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
        m_thread->wait();
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
    }

    void SystemController::startInitialiseHw()
    {
        m_worker->abort();
        m_thread->wait();
        m_worker->requestWork();
    }

    void SystemController::startConfigureHw()
    {
        m_worker->requestConfigureHw();
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
        //getCbcRegMap();
    }

    void SystemController::SendStatusMessage(QString msg)
    {
        emit notifyStatusMessage(msg);
    }

    void SystemController::getCbcRegMap()
    {
        return;
    }
        /*QVector<QVector<QVector<QVector<QMap<std::string, uint8_t>>>>> ShelveVector;
        QVector<QVector<QVector<QMap<std::string, uint8_t>>>> BoardVector;
        QVector<QVector<QMap<std::string, uint8_t>>> ModuleVector;
        QVector<QMap<std::string, uint8_t>> CbcVector;
        QMap<std::string, uint8_t> RegMap;

        for ( auto cShelve : m_worker->fShelveVector )
        {
            for ( auto cBoard : ( cShelve )->fBoardVector )
            {
                for ( auto cFe : cBoard.fModuleVector )
                {
                    for ( auto cCbc : cFe.fCbcVector )
                        //m_worker->fCbcInterface->re
                        //m_worker->fCbcInterface->WriteCbcReg( &cCbc, "VCth", uint8_t( cVcth ) );
                }
            }
        }
    }*/
}

