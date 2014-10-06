#include "provider.h"
#include <QString>
#include "Model/settings.h"
//#include "Model/systemcontrollersettings.h"
#include "utils.h"

#include <QDebug>

namespace GUI
{
    //SystemControllerSettings Provider::m_instanceControllerSettings;

    static QString SETTINGS_FILE=QString("HwDescription.json").toLatin1();
    std::unique_ptr<Settings> Provider::m_instanceSettings = nullptr;

    Settings &Provider::getSettingsAsSingleton()
    {
        if (m_instanceSettings == nullptr)
        {
            m_instanceSettings = Utils::make_unique<Settings>(nullptr, SETTINGS_FILE);
        }
        return *m_instanceSettings;
    }

    //std::unique_ptr<SystemControllerSettings> Provider::m_instanceControllerSettings(new SystemControllerSettings);



    //SystemControllerSettings &Provider::getSystemControllerSettings()
   // {
       // return *m_instanceControllerSettings;
   // }


}
