#include "startup.h"
#include "View/setuptab.h"
#include "View/cbcregisterstab.h"
#include "View/datatesttab.h"

#include "Macros/macrotesttab.h"

#include "View/mainview.h"
#include "Model/settings.h"
#include "Model/systemcontroller.h"
#include "Model/systemcontrollerworker.h"
#include "Model/datatest.h"
#include "ViewMgr/setuptabviewmanager.h"
#include "provider.h"
#include "ViewMgr/datatestviewmanager.h"

#include "utils.h"

#include <QDebug>
#include <memory>



namespace GUI
{
    Startup::Startup() :
        QObject(nullptr),

        m_setupTab(*new SetupTab(nullptr)), //nullptr - transferring ownership in mianview
        m_regTab(*new CbcRegistersTab(nullptr)),
        m_dataTab(*new DataTestTab(nullptr)),

        m_macroTestTab(*new MacroTestTab(nullptr)), //cheap quick non-std macro tab

        m_mainView(*new MainView(nullptr,
                                 m_setupTab,
                                 m_regTab,
                                 m_dataTab,
                                 m_macroTestTab)),

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
       // m_systemController->m
    }
}
