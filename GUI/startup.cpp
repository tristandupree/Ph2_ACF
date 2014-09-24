#include "startup.h"
#include "View/setuptab.h"
#include "View/cbcregisterstab.h"
#include "View/datatesttab.h"
#include "View/mainview.h"
#include "Model/settings.h"
#include "Model/systemcontroller.h"
#include "Model/datatest.h"
#include "ViewMgr/setuptabviewmanager.h"
#include "provider.h"
#include "ViewMgr/datatestviewmanager.h"

#include "utils.h"
#include <QDebug>



namespace GUI
{
    Startup::Startup() :
        QObject(nullptr),
        m_setupTab(*new SetupTab(nullptr)),
        m_regTab(*new CbcRegistersTab(nullptr)),
        m_dataTab(*new DataTestTab(nullptr)),
        m_mainView(*new MainView(nullptr,
                                 m_setupTab,
                                 m_regTab,
                                 m_dataTab)),

        m_systemController(new SystemController(this,
                                                 Provider::getSettingsAsSingleton())),

        m_dataTest(new DataTest(this,
                                *m_systemController)),

        m_setupTabVm(new SetupTabViewManager(this,
                                             m_setupTab,
                                             *m_systemController,
                                             Provider::getSettingsAsSingleton() )),
        m_dataTabVm(new DataTestViewManager(this,
                                            m_dataTab,
                                            *m_dataTest))

    {

    }

    Startup::~Startup(){
        qDebug() << "Destructing " << this;
        delete &m_mainView;
    }

    void Startup::show() const{
        m_mainView.show();
    }
}
