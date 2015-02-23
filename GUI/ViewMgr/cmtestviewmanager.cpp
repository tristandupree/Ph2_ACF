#pragma once
#include "cmtestviewmanager.h"
#include "View/cmtesttab.h"
#include "Model/cmtest.h"

namespace GUI {

    CmTestViewManager::CmTestViewManager(QObject *parent,
                                         CmTestTab &cmTab,
                                         CmTest &cm) :
        QObject(parent),
        m_cmTab(cmTab),
        m_cm(cm)
    {
        WireButtons();
        WireExternalCalls();
    }

    CmTestViewManager::~CmTestViewManager()
    {

    }


    void CmTestViewManager::WireButtons()
    {
        connect(&m_cmTab, SIGNAL(startCmTest()),
                &m_cm, SLOT(initialiseSettings()));
        connect(&m_cm, SIGNAL(getIsScan()),
                &m_cmTab, SLOT(getIsNoiseScan()));
        connect(&m_cmTab, SIGNAL(sendIsScan(bool)),
                &m_cm, SLOT(setIsScan(bool)));

    }

    void CmTestViewManager::WireExternalCalls()
    {
        connect(&m_cm, SIGNAL(startedCmTest()),
                this, SIGNAL(onCmTestStart()));
        connect(&m_cm, SIGNAL(finishedCmTest()),
                this, SIGNAL(onCmTestFinished()));

    }


}

