#include "setuptabviewmanager.h"
#include "utils.h"
#include "View/setuptab.h"
#include "Model/systemcontroller.h"
#include "Model/settings.h"
#include <QDebug>


namespace GUI
{
    SetupTabViewManager::SetupTabViewManager(QObject *parent,
                                             SetupTab& tab,
                                             SystemController& sysCtrl,
                                             Settings& config) :
        QObject(parent),
        m_setupTab(tab),
        m_systemController(sysCtrl)
    {
        WireMessages(config);
        WireSetupTabButtons(config);
        WireCallToOtherTabs();
    }

    SetupTabViewManager::~SetupTabViewManager()
    {
        qDebug() << "Destructing " << this;
    }

    void SetupTabViewManager::relayEnableAllTabs(const bool enable)
    {
        qDebug() << "Call made to other tabs" << enable;
        emit enableAlltabs(enable);
    }

    void SetupTabViewManager::WireMessages(Settings& config)
    {
        connect(&config, SIGNAL(notifyStatusMessage(QString)), //for QT4
                &m_setupTab, SLOT(onStatusUpdate(QString)) );

        connect(&m_systemController, SIGNAL(notifyStatusMessage(QString)),
                &m_setupTab, SLOT(onStatusUpdate(QString)));
    }

    void SetupTabViewManager::WireSetupTabButtons(Settings& config)
    {
        connect(&m_setupTab, SIGNAL(onBtnLoadSettingsClicked(bool)),
                &config, SLOT(onLoadButtonClicked(bool)) );

        connect(&config, SIGNAL(setHwTree(QStandardItemModel*)),
                &m_setupTab, SLOT(setHwTreeView(QStandardItemModel*)));

        connect(&m_setupTab, SIGNAL(onBtnInitClicked()),
                &m_systemController, SLOT(startInitialiseHw()));
        connect(&m_setupTab, SIGNAL(onBtnCfgClicked()),
                &m_systemController, SLOT(startConfigureHw()));

        connect(&m_systemController, SIGNAL(notifyInitFinished()),
                &m_setupTab, SLOT(onInitFinished()));
        connect(&m_systemController, SIGNAL(notifyConfigFinished()),
                &m_setupTab, SLOT(onConfigFinished()));
    }

    void SetupTabViewManager::WireCallToOtherTabs()
    {
        qDebug() << "Tabs wired";
        connect(&m_setupTab, SIGNAL(enableAllTabs(bool)),
                this, SIGNAL(enableAlltabs(bool)));
        //connect(&m_setupTab, SIGNAL(enableAllTabs(bool)),
                //this, SLOT(relayEnableAllTabs(bool)));
    }





}
