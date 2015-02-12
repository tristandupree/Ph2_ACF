#define _GUI
#include "startup.h"
#include "View/setuptab.h"
#include "View/cbcregisterstab.h"
#include "View/hybridtesttab.h"
#include "View/calibratetab.h"
#include "View/tbrowsertab.h"

#include "View/mainview.h"

#include "Model/settings.h"
#include "Model/systemcontroller.h"
#include "Model/systemcontrollerworker.h"
#include "Model/cbcregisters.h"
#include "Model/hybridtest.h"
#include "ViewMgr/setuptabviewmanager.h"
#include "provider.h"
#include "ViewMgr/cbcregviewmanager.h"
#include "ViewMgr/hybridtestviewmanager.h"
#include "ViewMgr/mainviewmanager.h"

#include "utils.h"

#include <QDebug>
#include <memory>



namespace GUI
{
    Startup::Startup() :
        QObject(nullptr),

        m_setupTab(*new SetupTab(nullptr)), //nullptr - transferring ownership in mainview
        m_regTab(*new CbcRegistersTab(nullptr)),
        m_hybridTab(*new HybridTestTab(nullptr)),
        m_calibrateTab(*new CalibrateTab(nullptr)),
        m_tbrowseTab(*new TBrowserTab(nullptr)),

        m_mainView(*new MainView(nullptr,
                                 m_setupTab,
                                 m_regTab,
                                 m_hybridTab,
                                 m_calibrateTab,
                                 m_tbrowseTab)),

        m_systemController(new SystemController(this,
                                                Provider::getSettingsAsSingleton())),
        m_cbcReg(new CbcRegisters(this,
                                  *m_systemController)),
        m_hybridTest(new HybridTest(this)),

        m_setupTabVm(*new SetupTabViewManager(this,
                                             m_setupTab,
                                             *m_systemController,
                                             Provider::getSettingsAsSingleton() )),
        m_cbcRegTabVm(*new CbcRegViewManager(this,
                                             m_regTab,
                                             *m_cbcReg)),
        m_hybridTabVm(*new HybridTestViewManager(this,
                                            m_hybridTab,
                                            *m_hybridTest)),


        m_mainViewVm(new MainViewManager(this,
                                         m_mainView,
                                         m_setupTabVm,
                                         m_cbcRegTabVm,
                                         m_hybridTabVm))
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
