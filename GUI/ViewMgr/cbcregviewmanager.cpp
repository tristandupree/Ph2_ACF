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
        connect(&m_cbcRegistersTab, SIGNAL(refreshCbcRegisters()),
                &m_cbcRegisters, SLOT(getCbcRegistersMap()));
        connect(&m_cbcRegisters, SIGNAL(sendCbcRegisterValue(int,std::map<std::string,CbcRegItem>)),
                &m_cbcRegistersTab, SLOT(createCbcRegisterValue(int,std::map<std::string,CbcRegItem>)));
        connect(&m_cbcRegistersTab, SIGNAL(sendCbcRegisters(int,std::vector<std::pair<std::string,std::uint8_t> >)),
                &m_cbcRegisters, SLOT(sendCbcRegisters(int,std::vector<std::pair<std::string,std::uint8_t> >)));
    }

    void CbcRegViewManager::WireExternalConnections()
    {
        connect(this, SIGNAL(sendInitialiseRegistersView()),
                &m_cbcRegisters, SLOT(getCbcRegistersMap()));
        connect(this, SIGNAL(on2CbcToggle(bool)),
                &m_cbcRegistersTab, SLOT(setupCbcRegGrid(bool)));
    }

}
