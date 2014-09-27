#include "startup.h"
#include "View/setuptab.h"
#include "View/cbcregisterstab.h"
#include "View/datatesttab.h"

#include "Macros/macrotesttab.h"

#include "View/mainview.h"
#include "Model/settings.h"
#include "Model/systemcontroller.h"
#include "Model/datatest.h"
#include "ViewMgr/setuptabviewmanager.h"
#include "provider.h"
#include "ViewMgr/datatestviewmanager.h"

#include "utils.h"
#include <QDebug>
#include <QThread>



namespace GUI
{
    Startup::Startup() :
        QObject(nullptr),
        //m_threadDataTest(new QThread()),

        m_setupTab(*new SetupTab(nullptr)), //nullptr as widget is added to a layout - transferring ownership
        m_regTab(*new CbcRegistersTab(nullptr)),
        m_dataTab(*new DataTestTab(nullptr)),

        m_macroTestTab(*new MacroTestTab(nullptr)), //cheap quick macro tab

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
        //auto thread = new QThread();
        //m_dataTest->moveToThread(thread);

    }

    Startup::~Startup(){
        //m_threadDataTest->wait();
        //delete m_threadDataTest;
        //qDebug()<<"Deleting thread "<<this->QObject::thread()->currentThreadId();
        qDebug() << "Destructing " << this;
        delete &m_mainView;
    }

    void Startup::show() const{
        m_mainView.show();
    }
}
