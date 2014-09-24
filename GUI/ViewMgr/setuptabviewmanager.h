#pragma once
#include <QObject>

#include <QStandardItemModel>

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

    private:

        SetupTab& m_setupTab;
        SystemController& m_systemController;

        void WireMessages(Settings &config);
        void WireSetupTabButtons(Settings &config);

        explicit SetupTabViewManager(const SetupTabViewManager& rhs) = delete;
        SetupTabViewManager& operator= (const SetupTabViewManager& rhs) = delete;
    };
}



