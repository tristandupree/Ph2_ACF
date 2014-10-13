#pragma once
#include <QDebug>
#include <QThread>
#include "Model/systemcontrollerworker.h"

//TODO Add proper destruct and safety methods to thread

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

namespace GUI
{

    SystemControllerWorker::SystemControllerWorker(QObject *parent,
                                                   Settings &config) :
        QObject(parent),
        m_Settings(config)
    {
        _working = false;
        _abort = false;
    }

    SystemControllerWorker::~SystemControllerWorker()
    {
        qDebug() << "Destructing System Controller Worker" << this;
    }

    void SystemControllerWorker::requestWork()
    {
        mutex.lock();
        _working = true;
        _abort = false;
        qDebug() << "Starting System Controller Worker on thread " << this;
        mutex.unlock();

        emit workRequested();
    }

    void SystemControllerWorker::requestConfigureHw()
    {
        mutex.lock();
        _working = true;
        _abort = false;
        mutex.unlock();

        emit workConfigureHwRequested();
    }

    void SystemControllerWorker::abort()
    {
        mutex.lock();
        if (_working) {
            _abort = true;
            qDebug()<<"Request worker aborting in Thread "<<thread()->currentThreadId();
        }
        mutex.unlock();
    }

    void SystemControllerWorker::InitializeHw()
    {
        mutex.lock();
        bool abort = _abort;
        mutex.unlock();

        if (abort) {
            return;
        }

        uint32_t cShelveId;
        uint32_t cBeId;
        uint32_t cModuleId;
        uint32_t cCbcId;
        uint32_t cFeId;
        uint32_t cFmcId;
        uint32_t cNShelve = 0;

        map_HwDescription = new QVariantMap();

        *map_HwDescription = m_Settings.getHwDescriptionMap();

        QVariantMap map_ShelveId = map_HwDescription->value("ShelveId").toMap();

        for (auto& sh_kv: map_ShelveId.keys())
        {
            if (abort) {
                break;
            }
            cShelveId=sh_kv.toUInt();
            fShelveVector.push_back((new Shelve(cShelveId)));

            QVariantMap map_BeBoardId = map_ShelveId.value(sh_kv).toMap().value("BeBoardId").toMap();

            for(auto& be_kv: map_BeBoardId.keys())
            {

                cBeId = be_kv.toUInt();
                BeBoard cBeBoard(cShelveId,cBeId);

                QVariantMap map_BeBoardIdValues = map_BeBoardId.value(be_kv).toMap();

                QVariantMap map_RegisterValues = map_BeBoardIdValues.value("RegisterName").toMap();

                for(auto& reg_kv : map_RegisterValues.keys()  )
                {
                    cBeBoard.setReg(reg_kv.toStdString(),map_RegisterValues.value(reg_kv).toUInt());
                }
                fShelveVector.at(cNShelve)->addBoard(cBeBoard);
                BeBoardFWInterface* cBeBoardFWInterface;

                qDebug() << map_BeBoardIdValues.value("boardType").toString();

                if(map_BeBoardIdValues.value("boardType").toString() == "GLIB")
                {
                    cBeBoardFWInterface = new GlibFWInterface("file://settings/connections_2CBC.xml",cBeId); //TODO - get rid of XML - get from JSON

                    fBeBoardFWMap[cBeId] = cBeBoardFWInterface;

                }

                if( map_BeBoardIdValues.contains("Module"))
                {

                    QVariantMap map_module_values = map_BeBoardIdValues.value("Module").toMap();

                    cModuleId = map_module_values.value("FeId").toUInt();
                    cFmcId = map_BeBoardId.value("FMCId").toInt();
                    cFeId = map_BeBoardId.value("FeId").toInt();
                    Module cModule(cShelveId,cBeId, cFmcId, cFeId,cModuleId);
                    fShelveVector[cNShelve]->getBoard(cBeId)->addModule(cModule);

                    int index(0);


                    for(auto& config_lv: map_module_values.value("CbcConfigFile").toList()) //could change this loop for indv. + global
                    {
                        Cbc cCbc(cShelveId,cBeId,cFmcId,cFeId,index,config_lv.toString().toStdString());
                        index++;

                        for(auto& cbcReg_kv : map_module_values.value("CbcRegisters").toMap().keys())
                        {
                            cCbc.setReg(cbcReg_kv.toStdString(), map_module_values.value("CbcRegisters").toMap().value(cbcReg_kv).toInt());
                        }
                        fShelveVector.at(cNShelve)->getBoard(cBeId)->getModule(cModuleId)->addCbc(cCbc);
                    }
                }
            }
        }
        fBeBoardInterface = new BeBoardInterface(fBeBoardFWMap);
        fCbcInterface = new CbcInterface(fBeBoardFWMap);

        mutex.lock();
        _working = false;
        mutex.unlock();
        emit finishedInitialiseHw();
    }

    void SystemControllerWorker::ConfigureHw()
    {
        qDebug() << "Starting System Controller Worker on thread " << this;
        mutex.lock();
        bool abort = _abort;
        mutex.unlock();

        if (abort) {
            return;
        }

        class Configurator: public HwDescriptionVisitor
        {
        private:
            Ph2_HwInterface::BeBoardInterface* fBeBoardInterface;
            Ph2_HwInterface::CbcInterface* fCbcInterface;
        public:
            Configurator( Ph2_HwInterface::BeBoardInterface* pBeBoardInterface, Ph2_HwInterface::CbcInterface* pCbcInterface ): fBeBoardInterface( pBeBoardInterface ), fCbcInterface( pCbcInterface ) {}
            void visit( BeBoard& pBoard ) {
                fBeBoardInterface->ConfigureBoard( &pBoard );
                qDebug() << "Configured Board " << int( pBoard.getBeId() );
            }
            void visit( Cbc& pCbc ) {
                fCbcInterface->ConfigureCbc( &pCbc );

                qDebug() << "Successfully configured Cbc " << int( pCbc.getCbcId() );
            }
        };

        Configurator cConfigurator( fBeBoardInterface, fCbcInterface );
        accept( cConfigurator );


        mutex.lock();
        _working = false;
        mutex.unlock();
        emit finishedConfigureHw();
    }


    void SystemControllerWorker::Run( BeBoard* pBeBoard, uint32_t pNthAcq )
    {
        fBeBoardInterface->Start( pBeBoard );
        fBeBoardInterface->ReadData( pBeBoard, pNthAcq, true );
        fBeBoardInterface->Stop( pBeBoard, pNthAcq );
    }

}
