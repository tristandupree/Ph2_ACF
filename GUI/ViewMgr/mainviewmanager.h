#pragma once
#include <QObject>

namespace GUI{

    class MainView;
    class SetupTabViewManager;
    class CbcRegViewManager;
    class HybridTestViewManager;

    class MainViewManager : public QObject
    {
        Q_OBJECT
    public:
        explicit MainViewManager(QObject *parent,
                                 MainView& mainView,
                                 SetupTabViewManager& setupVm,
                                 CbcRegViewManager& cbcVm,
                                 HybridTestViewManager& hybridVm);

        ~MainViewManager();
    private:
        MainView& m_mainView;
        SetupTabViewManager& m_setupVm;
        CbcRegViewManager& m_cbcRegVm;
        HybridTestViewManager& m_hybridTestVm;

        void WireSetupVmMessages();
        void WireHybridTestVmMessages();

        explicit MainViewManager(const MainViewManager& rhs) = delete;
        MainViewManager& operator= (const MainViewManager& rhs) = delete;
    };
}



