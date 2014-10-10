#pragma once
#include <QObject>
#include <memory>

namespace GUI{


    class SetupTab;
    class CbcRegistersTab;
    class DataTestTab;

    class MacroTestTab; //cheap non-std tab

    class MainView;

    class Settings;

    class SystemController;

    class DataTest;
    class SetupTabViewManager;
    class DataTestViewManager;




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

        MacroTestTab& m_macroTestTab; //cheap non-std tab

        MainView& m_mainView;
        Settings* m_settings;

        SystemController* m_systemController;

        DataTest* m_dataTest;

        SetupTabViewManager* m_setupTabVm;
        DataTestViewManager* m_dataTabVm;

        explicit Startup(const Startup& rhs) = delete;
        Startup& operator= (const Startup& rhs) = delete;
    };
}


