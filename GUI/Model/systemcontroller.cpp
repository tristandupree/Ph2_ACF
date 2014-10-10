#include "Model/systemcontroller.h"
#include "settings.h"
#include "Model/systemcontrollerworker.h"

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
        m_SystemControllerWorker(new SystemControllerWorker(config))
    {
    }

    SystemController::~SystemController()
    {
        qDebug() << "Destructing " << this;
    }

    void SystemController::startInitialiseHw()
    {
        InitialiseHw();
        SendStatusMessage(tr("Initialised settings"));
    }

    void SystemController::startConfigureHw()
    {
        ConfigureHw();
        SendStatusMessage(tr("Hardware configured"));
    }

    void SystemController::InitialiseHw()
    {
        m_SystemControllerWorker->InitializeHw();
    }

    void SystemController::ConfigureHw()
    {
        m_SystemControllerWorker->ConfigureHw();
    }

    void SystemController::SendStatusMessage(QString msg)
    {
        emit notifyStatusMessage(msg);
    }
}


