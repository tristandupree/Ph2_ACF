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

       // SystemControllerWorker &getSystemControllerWorker() const {return *m_SystemControllerWorker;}

        void requestWork();
        void abort();

    ~SystemController();


    signals:
        void notifyStatusMessage(QString msg);
        void notifyConfigFinished();
        void notifyInitFinished();

    public slots:

        void startInitialiseHw();
        void startConfigureHw();
        void finishInitialiseHw();
        void finishConfigureHw();


    private:

        Settings& m_Settings;
        QThread *m_thread;
        SystemControllerWorker *m_worker;

        void SendStatusMessage(QString msg);

        void WireThreadConnections();

        explicit SystemController(const SystemController& rhs) = delete;
        SystemController& operator= (const SystemController& rhs) = delete;
    };
}



