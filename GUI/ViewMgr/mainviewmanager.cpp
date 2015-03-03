#include "mainviewmanager.h"
#include "View/mainview.h"
#include "ViewMgr/cbcregviewmanager.h"
#include "ViewMgr/hybridtestviewmanager.h"
#include "ViewMgr/setuptabviewmanager.h"
#include "ViewMgr/calibrateviewmanager.h"
#include "ViewMgr/cmtestviewmanager.h"

#include <QDebug>

namespace GUI
{
    MainViewManager::MainViewManager(QObject *parent,
                                     MainView &mainView,
                                     SetupTabViewManager &setupVm,
                                     CbcRegViewManager &cbcVm,
                                     HybridTestViewManager &hybridVm,
                                     CalibrateViewManager &calibVm,
                                     CmTestViewManager &cmVm) :
        QObject(parent),
        m_mainView(mainView),
        m_setupVm(setupVm),
        m_cbcRegVm(cbcVm),
        m_hybridTestVm(hybridVm),
        m_calibrateVm(calibVm),
        m_cmVm(cmVm)
    {
        WireSetupVmMessages();
        WireLaunchButtons();
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

    void MainViewManager::WireLaunchButtons()
    {
        connect(&m_hybridTestVm, SIGNAL(startedHybridTest()),
                &m_calibrateVm, SIGNAL(disableLaunch()));
        connect(&m_hybridTestVm, SIGNAL(finishedHybridTest()),
                &m_calibrateVm, SIGNAL(enableLaunch()));
        connect(&m_hybridTestVm, SIGNAL(startedHybridTest()),
                &m_cmVm, SIGNAL(disableLaunch()));
        connect(&m_hybridTestVm, SIGNAL(finishedHybridTest()),
                &m_cmVm, SIGNAL(enableLaunch()));

        connect(&m_calibrateVm, SIGNAL(startedCalibration()),
                &m_hybridTestVm, SIGNAL(disableLaunch()));
        connect(&m_calibrateVm, SIGNAL(finishedCalibration()),
                &m_hybridTestVm, SIGNAL(enableLaunch()));
        connect(&m_calibrateVm, SIGNAL(startedCalibration()),
                &m_cmVm, SIGNAL(disableLaunch()));
        connect(&m_calibrateVm, SIGNAL(finishedCalibration()),
                &m_cmVm, SIGNAL(enableLaunch()));

        connect(&m_cmVm, SIGNAL(onCmTestStart()),
                &m_hybridTestVm, SIGNAL(disableLaunch()));
        connect(&m_cmVm, SIGNAL(onCmTestFinished()),
                &m_hybridTestVm, SIGNAL(enableLaunch()));
        connect(&m_cmVm, SIGNAL(onCmTestStart()),
                &m_calibrateVm, SIGNAL(disableLaunch()));
        connect(&m_cmVm, SIGNAL(onCmTestFinished()),
                &m_calibrateVm, SIGNAL(enableLaunch()));

    }
}
