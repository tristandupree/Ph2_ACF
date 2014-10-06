#pragma once
#include <QWidget>
#include <memory>
//#include "Model/systemcontrollersettings.h"

namespace GUI{

    class Settings;
    //class SystemControllerSettings;

    class Provider final
    {
    public:
        Provider();
        //static SystemControllerSettings m_instanceControllerSettings;
        static Settings& getSettingsAsSingleton();

    private:

        static std::unique_ptr<Settings> m_instanceSettings;
        //static std::unique_ptr<SystemControllerSettings> m_instanceControllerSettings;
        //static SystemControllerSettings m_instanceControllerSettings;
        explicit Provider(const Provider& rhs) = delete;
        Provider& operator = (const Provider& rhs) = delete;
    };
}



