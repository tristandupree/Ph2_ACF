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
        //qRegisterMetaType<std::map<std::string,CbcRegItem>>("std::map<std::string,CbcRegItem>");
    }

    void CbcRegisters::getObjects()
    {
        fBeBoardInterface = m_systemController.getBeBoardInterface();
        fCbcInterface = m_systemController.getCbcInterface();
        fShelveVector = m_systemController.getfShelveVector();
        fBeBoardFWMap = m_systemController.getBeBoardFWMap();
    }

    void CbcRegisters::getCbcRegistersMap()
    {

        CbcRegItem test;
        test.fAddress=5;
        test.fDefValue=3;
        test.fPage = 0;
        test.fValue=10;

        std::map<std::string, CbcRegItem> regMap = { {"0", test } };
        qDebug() << QString::number(test.fAddress);
        emit sendCbcRegisterValue(0, regMap);
        //getObjects();

        /*for ( auto cShelve : fShelveVector )
        {
            for ( auto cBoard : ( cShelve )->fBoardVector )
            {
                for ( auto cFe : cBoard.fModuleVector )
                {
                    fCbcInterface->ReadAllCbc(&cFe);

                    for ( auto cCbc : cFe.fCbcVector )
                    {
                        //std::map<std::string, CbcRegItem> regMap = cCbc.getRegMap();

                        CbcRegItem test;
                        test.fAddress=1;
                        test.fDefValue=3;
                        test.fPage = 0;
                        test.fValue=10;

                        std::map<std::string, CbcRegItem> regMap = { {"0", test } };


                        //int nCbc = int(cCbc.getCbcId());

                        //emit(nCbc, regMap);
                        emit sendCbcRegisterValue(0, regMap);
                        qDebug() << "emitted";
                    }
                }
            }
        }*/
    }
}
