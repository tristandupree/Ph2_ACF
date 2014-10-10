//NEEDS MASSIVE TIDY

#pragma once
#include <QObject>
#include <iostream>

#include <QVariantMap>

#include "settings.h"
#include "Model/systemcontrollerworker.h"

namespace GUI{

    class Settings;

    class SystemController : public QObject
    {
        Q_OBJECT
    public:
        explicit SystemController(QObject *parent,
                                  Settings &config);

        void Run(BeBoard *pBeBoard, uint32_t pNthAcq);

    ~SystemController();


    signals:
        void notifyStatusMessage(QString msg);

    public slots:

        void startInitialiseHw();
        void startConfigureHw();


    private:

        Settings& m_Settings;
        SystemControllerWorker* m_SystemControllerWorker;

        void InitialiseHw();
        void ConfigureHw();
        void SendStatusMessage(QString msg);

        explicit SystemController(const SystemController& rhs) = delete;
        SystemController& operator= (const SystemController& rhs) = delete;
    };
}



