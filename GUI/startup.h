#pragma once
#include <QObject>
#include <memory>

namespace GUI{

    class SetupTab;
    class CbcRegistersTab;
    class DataTestTab;

    class MainView;

    class Settings;

    class SystemController;
    class CbcRegisters;
    class DataTest;
    class SetupTabViewManager;
    class CbcRegViewManager;
    class DataTestViewManager;

    class MainViewManager;

    class Startup : public QObject
    {
        Q_OBJECT
    public:
        explicit Startup();
        void show() const ;

        ~Startup();
    private:
        SetupTab& m_setupTab;
        CbcRegistersTab& m_regTab;
        DataTestTab& m_dataTab;

        MainView& m_mainView;
        Settings* m_settings;

        SystemController* m_systemController;

        CbcRegisters* m_cbcReg;
        DataTest* m_dataTest;

        SetupTabViewManager& m_setupTabVm;
        CbcRegViewManager& m_cbcRegTabVm;
        DataTestViewManager& m_dataTabVm;

        MainViewManager* m_mainViewVm;

        explicit Startup(const Startup& rhs) = delete;
        Startup& operator= (const Startup& rhs) = delete;
    };
}


