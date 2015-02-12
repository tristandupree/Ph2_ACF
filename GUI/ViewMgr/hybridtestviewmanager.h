#pragma once
#include <QObject>
#include <QThread>
#include "Model/hybridtest.h"

namespace GUI{

    class HybridTestTab;
    class HybridTest;

    class HybridTestViewManager : public QObject
    {
        Q_OBJECT
    public:
        explicit HybridTestViewManager(QObject *parent,
                                     HybridTestTab& hybridTab,
                                     HybridTest &hybridTest);
    ~HybridTestViewManager();

    signals:
        void disableLaunch();
        void enableLaunch();

    private:

        HybridTestTab& m_hybridTestTab;
        HybridTest& m_hybridTest;

        void WireButtons();
        void WireExternalCalls();

        explicit HybridTestViewManager(const HybridTestViewManager& rhs) = delete;
        HybridTestViewManager& operator= (const HybridTestViewManager& rhs) = delete;
    };
}



