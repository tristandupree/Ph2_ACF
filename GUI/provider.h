#pragma once
#include <QWidget>
#include <memory>

namespace GUI{

    class Settings;
    class SystemController;

    class Provider final
    {
    public:
        Provider();
        static Settings& getSettingsAsSingleton();
        static SystemController& getSystemControllerAsSingleton();

    private:

        static std::unique_ptr<Settings> m_instanceSettings;

        explicit Provider(const Provider& rhs) = delete;
        Provider& operator = (const Provider& rhs) = delete;
    };
}



