#pragma once
#include <QObject>

namespace GUI{

    class MainView;
    class SetupTabViewManager;
    class CbcRegViewManager;
    class DataTestViewManager;

    class MainViewManager : public QObject
    {
        Q_OBJECT
    public:
        explicit MainViewManager(QObject *parent,
                                 MainView& mainView,
                                 SetupTabViewManager& setupVm,
                                 CbcRegViewManager& cbcVm,
                                 DataTestViewManager& dataVm);

        ~MainViewManager();
    private:
        MainView& m_mainView;
        SetupTabViewManager& m_setupVm;
        CbcRegViewManager& m_cbcRegVm;
        DataTestViewManager& m_dataTestVm;

        void WireSetupVmMessages();
        void WireDataTestVmMessages();

        explicit MainViewManager(const MainViewManager& rhs) = delete;
        MainViewManager& operator= (const MainViewManager& rhs) = delete;
    };
}



