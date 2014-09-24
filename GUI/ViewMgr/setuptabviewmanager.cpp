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
    }

    SetupTabViewManager::~SetupTabViewManager()
    {
        qDebug() << "Destructing " << this;
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

    }





}
