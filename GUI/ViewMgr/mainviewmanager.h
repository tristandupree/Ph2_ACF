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
        CbcRegViewManager& m_regVm;
        DataTestViewManager& m_dataTestVm;
        //MacroTestTab& m_macroVm;

        void WireVmMessages();

        explicit MainViewManager(const MainViewManager& rhs) = delete;
        MainViewManager& operator= (const MainViewManager& rhs) = delete;
    };
}



