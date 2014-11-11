#pragma once
#include <QObject>
#include <iostream>

#include <QVariantMap>

#include "settings.h"
#include "Model/systemcontrollerworker.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

namespace GUI{

    class Settings;

    class SystemController : public QObject
    {
        Q_OBJECT
    public:
        explicit SystemController(QObject *parent,
                                  Settings &config);

        void Run(BeBoard *pBeBoard, uint32_t pNthAcq);

        void requestWork();
        void abort();
        BeBoardInterface* getBeBoardInterface() const {return (m_worker->fBeBoardInterface);}
        CbcInterface* getCbcInterface() const {return (m_worker->fCbcInterface);}
        ShelveVec getfShelveVector() const {return (m_worker->fShelveVector);}
        BeBoardFWMap getBeBoardFWMap() const {return (m_worker->fBeBoardFWMap);}

        //SystemControllerWorker* getSysCtrlWorker() const {return m_worker;}

        SystemControllerWorker *m_worker;

        ~SystemController();

    signals:
        void notifyStatusMessage(QString msg);
        void notifyConfigFinished();
        void notifyInitFinished();
        void sendInitialiseRegistersView();

    public slots:

        void startInitialiseHw();
        void startConfigureHw();
        void finishInitialiseHw();
        void finishConfigureHw();

        void onAccept(HwDescriptionVisitor pVisitor);


    private:

        Settings  &m_Settings;
        QThread *m_thread;
       // SystemControllerWorker *m_worker;

        void SendStatusMessage(QString msg);

        void WireThreadConnections();

        explicit SystemController(const SystemController& rhs) = delete;
        SystemController& operator= (const SystemController& rhs) = delete;
    };
}



