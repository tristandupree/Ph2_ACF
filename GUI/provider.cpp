#include "provider.h"
#include <QString>
#include "Model/settings.h"
#include "utils.h"

#include <QDebug>

namespace GUI
{
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
}
