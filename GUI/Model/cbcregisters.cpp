#include "cbcregisters.h"
#include "Model/systemcontroller.h"
#include "../HWDescription/Cbc.h"
#include "../HWDescription/CbcRegItem.h"
#include <QDebug>

namespace GUI
{
    CbcRegisters::CbcRegisters(QObject *parent,
                               SystemController &sysCtrl) :
        QObject(parent),
        m_systemController(sysCtrl)
    {
    }

    void CbcRegisters::getObjects()
    {
        fBeBoardInterface = m_systemController.getBeBoardInterface();
        fCbcInterface = m_systemController.getCbcInterface();
        fShelveVector = m_systemController.getfShelveVector();
        fBeBoardFWMap = m_systemController.getBeBoardFWMap();
    }

    void CbcRegisters::WireThreadConnections()
    {

    }

    void CbcRegisters::getCbcRegistersMap()
    {
        getObjects();

        for ( auto cShelve : fShelveVector )
        {
            for ( auto cBoard : ( cShelve )->fBoardVector )
            {
                for ( auto cFe : cBoard->fModuleVector )
                {
                    fCbcInterface->ReadAllCbc(cFe);

                    for ( auto cCbc : cFe->fCbcVector )
                    {
                        emit sendCbcRegisterValue(cCbc->getCbcId(), cCbc->getRegMap());
                    }
                }
            }
        }
    }

    void CbcRegisters::sendCbcRegisters(const int cbc, std::vector<std::pair<std::string, std::uint8_t>> mapReg)
    {
        for ( auto cShelve : fShelveVector )
        {
            for ( auto cBoard : ( cShelve )->fBoardVector )
            {
                for ( auto cFe : cBoard->fModuleVector )
                {
                    for ( auto cCbc : cFe->fCbcVector )
                    {
                        if (cCbc->getCbcId()==cbc)
                        {
                            fCbcInterface->WriteCbcMultReg(cCbc, mapReg );
                            qDebug() << "values written";
                        }
                    }
                }
            }
        }
    }
}
