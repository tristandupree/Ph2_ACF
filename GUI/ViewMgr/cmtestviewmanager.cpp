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

    }

    CmTestViewManager::~CmTestViewManager()
    {

    }

    void CmTestViewManager::WireConnections()
    {

    }


}

