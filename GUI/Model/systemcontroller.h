//NEEDS MASSIVE TIDY

#pragma once
#include <QObject>
#include <iostream>

#include <QVariantMap>

#include "settings.h"
#include "Model/systemcontrollersettings.h"

namespace GUI{

    class Settings;

    class SystemController : public QObject//, public SystemControllerSettings
    {
        Q_OBJECT
    public:
        explicit SystemController(QObject *parent,
                                  Settings &config,
                                  SystemControllerSettings &ctrlConfig);

        void Run(BeBoard *pBeBoard, uint32_t pNthAcq);

    ~SystemController();


    signals:
        void notifyStatusMessage(QString msg);

    public slots:

        void startInitialiseHw();
        void startConfigureHw();


    private:

        void InitialiseHw();
        void ConfigureHw();

        uint32_t cShelveId;
        uint32_t cBeId;
        uint32_t cModuleId;
        uint32_t cCbcId;
        uint32_t cFeId;
        uint32_t cFmcId;
        uint32_t cNShelve;

        Settings& m_Settings;
        SystemControllerSettings& m_systemSettings;


        QVariantMap* map_ShelveId; //TODO don't pass in like this
        void SendStatusMessage(QString msg);

        explicit SystemController(const SystemController& rhs) = delete;
        SystemController& operator= (const SystemController& rhs) = delete;
    };
}



