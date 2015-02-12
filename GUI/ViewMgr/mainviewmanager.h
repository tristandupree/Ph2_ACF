#pragma once
#include <QObject>

namespace GUI{

    class MainView;
    class SetupTabViewManager;
    class CbcRegViewManager;
    class HybridTestViewManager;
    class CalibrateViewManager;

    class MainViewManager : public QObject
    {
        Q_OBJECT
    public:
        explicit MainViewManager(QObject *parent,
                                 MainView& mainView,
                                 SetupTabViewManager& setupVm,
                                 CbcRegViewManager& cbcVm,
                                 HybridTestViewManager& hybridVm,
                                 CalibrateViewManager& calibVm);

        ~MainViewManager();
    private:
        MainView& m_mainView;
        SetupTabViewManager& m_setupVm;
        CbcRegViewManager& m_cbcRegVm;
        HybridTestViewManager& m_hybridTestVm;
        CalibrateViewManager& m_calibrateVm;

        void WireSetupVmMessages();


        explicit MainViewManager(const MainViewManager& rhs) = delete;
        MainViewManager& operator= (const MainViewManager& rhs) = delete;
    };
}



