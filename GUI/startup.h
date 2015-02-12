#pragma once
#include <QObject>
#include <memory>

namespace GUI{

    class SetupTab;
    class CbcRegistersTab;
    class HybridTestTab;
    class CalibrateTab;
    class TBrowserTab;

    class MainView;

    class Settings;

    class SystemController;
    class CbcRegisters;
    class HybridTest;
    class Calibrate;

    class SetupTabViewManager;
    class CbcRegViewManager;
    class HybridTestViewManager;
    class CalibrateViewManager;

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
        HybridTestTab& m_hybridTab;
        CalibrateTab& m_calibrateTab;
        TBrowserTab& m_tbrowseTab;

        MainView& m_mainView;
        Settings* m_settings;

        SystemController* m_systemController;

        CbcRegisters* m_cbcReg;
        HybridTest* m_hybridTest;
        Calibrate* m_calibrate;

        SetupTabViewManager& m_setupTabVm;
        CbcRegViewManager& m_cbcRegTabVm;
        HybridTestViewManager& m_hybridTabVm;
        CalibrateViewManager& m_calibrateTabVm;

        MainViewManager* m_mainViewVm;

        explicit Startup(const Startup& rhs) = delete;
        Startup& operator= (const Startup& rhs) = delete;
    };
}


