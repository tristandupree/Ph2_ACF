#pragma once
#include <QWidget>

namespace GUI{
    class SystemControllerSettings
    {
    public:
        SystemControllerSettings();

    private:
        explicit SystemControllerSettings(const SystemControllerSettings& rhs) = delete;
        SystemControllerSettings& operator= (const SystemControllerSettings& rhs) = delete;
    };
}



