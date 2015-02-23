#pragma once

#include <QObject>

namespace GUI {
    class CmTestTab;
    class CmTest;

    class CmTestViewManager : public QObject
    {
        Q_OBJECT
    public:
        explicit CmTestViewManager(QObject *parent,
                                   CmTestTab& cmTab,
                                   CmTest& cm);
        ~CmTestViewManager();

    signals:

    public slots:


    private:
        CmTestTab& m_cmTab;
        CmTest& m_cm;

        void WireConnections();

        explicit CmTestViewManager(const CmTestViewManager& rhs) = delete;
        CmTestViewManager& operator= (const CmTestViewManager& rhs) = delete;
    };
}
