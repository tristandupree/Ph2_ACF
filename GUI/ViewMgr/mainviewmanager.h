#pragma once

#include <QObject>

namespace GUI{

    class MainView;
    class SetupTabViewManager;
    class CbcRegViewManager;
    class HybridTestViewManager;
    class CalibrateViewManager;
    class CmTestViewManager;

    class MainViewManager : public QObject
    {
        Q_OBJECT
    public:
        explicit MainViewManager(QObject *parent,
                                 MainView& mainView,
                                 SetupTabViewManager& setupVm,
                                 CbcRegViewManager& cbcVm,
                                 HybridTestViewManager& hybridVm,
                                 CalibrateViewManager& calibVm,
                                 CmTestViewManager &cmVm);

        ~MainViewManager();
    private:
        MainView& m_mainView;
        SetupTabViewManager& m_setupVm;
        CbcRegViewManager& m_cbcRegVm;
        HybridTestViewManager& m_hybridTestVm;
        CalibrateViewManager& m_calibrateVm;
        CmTestViewManager& m_cmVm;

        void WireSetupVmMessages();
        void WireLaunchButtons();


        explicit MainViewManager(const MainViewManager& rhs) = delete;
        MainViewManager& operator= (const MainViewManager& rhs) = delete;
    };
}



