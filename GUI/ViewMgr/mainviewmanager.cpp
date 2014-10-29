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
        m_cbcRegVm(cbcVm),
        m_dataTestVm(dataVm)
    {
        WireSetupVmMessages();
        WireDataTestVmMessages();
    }

    MainViewManager::~MainViewManager()
    {
        qDebug() << "Destructing " << this;
    }

    void MainViewManager::WireSetupVmMessages()
    {
        connect(&m_setupVm, SIGNAL(enableAlltabs(bool)),
                &m_mainView, SLOT(enableAllTabsSlot(bool)));

        connect(&m_setupVm, SIGNAL(on2CbcToggle(bool)),
                &m_dataTestVm, SIGNAL(on2CbcToggle(bool)));

        connect(&m_setupVm, SIGNAL(on2CbcToggle(bool)),
                &m_cbcRegVm, SIGNAL(on2CbcToggle(bool)));
        connect(&m_setupVm, SIGNAL(notifyConfigFinished()),
                &m_cbcRegVm, SIGNAL(notifyConfigFinished()));
    }

    void MainViewManager::WireDataTestVmMessages()
    {
        connect(&m_dataTestVm, SIGNAL(sendAccept(HwDescriptionVisitor)),
                &m_setupVm, SIGNAL(sendAccept(HwDescriptionVisitor)));

    }
}
