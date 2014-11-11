#include "cbcregviewmanager.h"
#include "View/cbcregisterstab.h"
#include "Model/cbcregisters.h"

#include <QDebug>

namespace GUI
{
    CbcRegViewManager::CbcRegViewManager(QObject *parent,
                                         CbcRegistersTab& cbcRegTab,
                                         CbcRegisters& cbcReg) :
        QObject(parent),
        m_cbcRegistersTab(cbcRegTab),
        m_cbcRegisters(cbcReg)
    {
        WireConnections();
        WireExternalConnections();
    }

    CbcRegViewManager::~CbcRegViewManager()
    {
        qDebug() << "Destructing " << this;
    }

    void CbcRegViewManager::WireConnections()
    {
        connect(&m_cbcRegisters, SIGNAL(sendCbcRegisterValue(int,std::map<std::string,CbcRegItem>)),
                &m_cbcRegistersTab, SLOT(createCbcRegisterValue(int,std::map<std::string,CbcRegItem>)));
    }

    void CbcRegViewManager::WireExternalConnections()
    {
        connect(this, SIGNAL(sendInitialiseRegistersView()),
                &m_cbcRegisters, SLOT(getCbcRegistersMap()));
        connect(this, SIGNAL(on2CbcToggle(bool)),
                &m_cbcRegistersTab, SLOT(setupCbcRegGrid(bool)));
    }

}
