#include "mainviewmanager.h"
#include "View/mainview.h"
#include "ViewMgr/cbcregviewmanager.h"
#include "ViewMgr/hybridtestviewmanager.h"
#include "ViewMgr/setuptabviewmanager.h"

#include <QDebug>

namespace GUI
{
    MainViewManager::MainViewManager(QObject *parent, MainView &mainView,
                                     SetupTabViewManager &setupVm,
                                     CbcRegViewManager &cbcVm,
                                     HybridTestViewManager &hybridVm) :
        QObject(parent),
        m_mainView(mainView),
        m_setupVm(setupVm),
        m_cbcRegVm(cbcVm),
        m_hybridTestVm(hybridVm)
    {
        WireSetupVmMessages();
        WireHybridTestVmMessages();
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
                &m_hybridTestVm, SIGNAL(on2CbcToggle(bool)));

        connect(&m_setupVm, SIGNAL(on2CbcToggle(bool)),
                &m_cbcRegVm, SIGNAL(on2CbcToggle(bool)));
        connect(&m_setupVm, SIGNAL(sendInitialiseRegistersView()),
                &m_cbcRegVm, SIGNAL(sendInitialiseRegistersView()));
    }

    void MainViewManager::WireHybridTestVmMessages()
    {
        connect(&m_hybridTestVm, SIGNAL(sendAccept(HwDescriptionVisitor)),
                &m_setupVm, SIGNAL(sendAccept(HwDescriptionVisitor)));

    }
}
