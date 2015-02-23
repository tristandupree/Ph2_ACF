#pragma once
#include "cbcregisterworker.h"
#include "Model/systemcontroller.h"
#include <QThread>


using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

namespace GUI
{
    CbcRegisterWorker::CbcRegisterWorker(QObject *parent,
                                         SystemController &sysCtrl) :
        QObject(parent),
        m_systemController(sysCtrl)
    {
    }
    CbcRegisterWorker::~CbcRegisterWorker()
    {
        qDebug() << "Destructing CbcRegister Worker" << this;
    }

    void CbcRegisterWorker::doWork()
    {
        fBeBoardInterface = m_systemController.getBeBoardInterface();
        fCbcInterface = m_systemController.getCbcInterface();
        fShelveVector = m_systemController.getfShelveVector();
        fBeBoardFWMap = m_systemController.getBeBoardFWMap();
    }

    void CbcRegisterWorker::requestWork()
    {
        qDebug()<<"Request worker start in Thread "<<thread()->currentThreadId();
        emit workRequested();
    }

    void CbcRegisterWorker::abort()
    {
        //TODO
        /*mutex.lock();
        if (_working) {
            _abort = true;
            qDebug()<<"Request worker aborting in Thread "<<thread()->currentThreadId();
        }
        mutex.unlock();*/
    }

    void CbcRegisterWorker::getObjects()
    {
        fBeBoardInterface = m_systemController.getBeBoardInterface();
        fCbcInterface = m_systemController.getCbcInterface();
        fShelveVector = m_systemController.getfShelveVector();
        fBeBoardFWMap = m_systemController.getBeBoardFWMap();
    }

    void CbcRegisterWorker::getCbcRegistersMap()
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
                        //qDebug() << "emitting for " << cCbc->getCbcId();
                        emit sendCbcRegisterValue(cCbc->getCbcId(), cCbc->getRegMap());
                    }
                }
            }
        }
    }

    void CbcRegisterWorker::sendCbcRegisters(const int cbc, std::vector<std::pair<std::string, std::uint8_t>> mapReg)
    {
        getObjects();

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
