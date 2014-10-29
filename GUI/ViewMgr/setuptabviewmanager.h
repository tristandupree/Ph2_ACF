#pragma once
#include <QObject>
#include "Model/systemcontroller.h"

//#include <QStandardItemModel>

namespace GUI{
    class SetupTab;
    class SystemController;
    class Settings;


    class SetupTabViewManager : public QObject
    {
        Q_OBJECT
    public:
        explicit SetupTabViewManager(QObject *parent,
                                     SetupTab& tab,
                                     SystemController& sysCtrl,
                                     Settings& config);
    ~SetupTabViewManager();

    signals:
        void notifyStatusUpdated(const QString& value);
        void enableAlltabs(const bool enable);
        void on2CbcToggle(const bool);
        void notifyConfigFinished();
        void sendAccept(HwDescriptionVisitor pVisitor);

    private:

        SetupTab& m_setupTab;
        SystemController& m_systemController;

        void WireMessages(Settings &config);
        void WireSetupTabButtons(Settings &config);
        void WireCallToOtherTabs();
        void WireCallFromOtherTabs();

        explicit SetupTabViewManager(const SetupTabViewManager& rhs) = delete;
        SetupTabViewManager& operator= (const SetupTabViewManager& rhs) = delete;
    };
}



