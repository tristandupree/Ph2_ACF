#include "mainviewmanager.h"
#include "View/mainview.h"
#include "ViewMgr/cbcregviewmanager.h"
#include "ViewMgr/datatestviewmanager.h"
#include "ViewMgr/setuptabviewmanager.h"

#include <QDebug>

namespace GUI
{
    MainViewManager::MainViewManager(QObject *parent, MainView &mainView,
                                     SetupTabViewManager &setupVm,
                                     CbcRegViewManager &cbcVm,
                                     DataTestViewManager &dataVm) :
        QObject(parent),
        m_mainView(mainView),
        m_setupVm(setupVm),
        m_regVm(cbcVm),
        m_dataTestVm(dataVm)
    {
        WireVmMessages();
    }

    MainViewManager::~MainViewManager()
    {
        qDebug() << "Destructing " << this;
    }

    void MainViewManager::WireVmMessages()
    {
        connect(&m_setupVm, SIGNAL(enableAlltabs(bool)),
                &m_mainView, SLOT(enableAllTabsSlot(bool)));

        connect(&m_setupVm, SIGNAL(onBtnLoadClicked(bool)),
                &m_dataTestVm, SIGNAL(onBtnLoadClicked(bool)));
    }
}
