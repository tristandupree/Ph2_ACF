#include "systemcontroller.h"
#include "settings.h"

#include "../HWInterface/BeBoardFWInterface.h"
#include "../HWDescription/Shelve.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include <iostream>
#include <vector>
#include <map>
#include <stdlib.h>
#include <string.h>

#include"provider.h"

#include<QDebug>

#include <QVariantMap>

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;


namespace GUI
{
    SystemController::SystemController(QObject *parent,
                                       Settings &config) :
        QObject(parent),
        cShelveId(0),
        cBeId(0),
        cModuleId(0),
        cCbcId(0),
        cNShelve(0),
        cFeId(0),
        cFmcId(0),
        map_ShelveId(config.getshelveIdMap())

    {
    }

    SystemController::~SystemController()
    {
        qDebug() << "Destructing " << this;
    }

    void SystemController::startInitialiseHw()
    {
        InitialiseHw();
        SendStatusMessage(tr("Initialised settings"));
    }

    void SystemController::startConfigureHw()
    {
        ConfigureHw();
        SendStatusMessage(tr("Hardware configured"));
    }

    void SystemController::InitialiseHw()
    {

        for (auto& sh_kv: map_ShelveId.keys())
        {

            cShelveId=sh_kv.toUInt();
            fShelveVec.push_back((new Shelve(cShelveId)));

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
                fShelveVec[cNShelve]->addBoard(cBeBoard);
                BeBoardFWInterface* cBeBoardFWInterface;

                if(map_BeBoardIdValues.value("boardType").toString() == "GLIB")
                {
                    cBeBoardFWInterface = new GlibFWInterface("file://settings/connections_2CBC.xml",cBeId); //TODO - get rid of XML
                    fBeBoardFWMap[cBeId] = cBeBoardFWInterface;
                }

                if( map_BeBoardIdValues.contains("Module"))
                {
                    QVariantMap map_module_values = map_BeBoardIdValues.value("Module").toMap();

                    cModuleId = map_module_values.value("FeId").toUInt();
                    cFmcId = map_BeBoardId.value("FMCId").toInt();
                    cFeId = map_BeBoardId.value("FeId").toInt();
                    Module cModule(cShelveId,cBeId, cFmcId, cFeId,cModuleId);
                    fShelveVec[cNShelve]->getBoard(cBeId)->addModule(cModule);

                    int index(0);

                    for(auto& config_lv: map_module_values.value("CbcConfigFile").toList()) //could change this loop for indv. + global
                    {
                        Cbc cCbc(cShelveId,cBeId,cFmcId,cFeId,index,config_lv.toString().toStdString());
                        index++;

                        for(auto& cbcReg_kv : map_module_values.value("CbcRegisters").toMap().keys())
                        {
                            cCbc.setReg(cbcReg_kv.toStdString(), map_module_values.value("CbcRegisters").toMap().value(cbcReg_kv).toInt());
                        }
                        fShelveVec[cNShelve]->getBoard(cBeId)->getModule(cModuleId)->addCbc(cCbc);
                    }
                }
            }
        }

        fBeBoardInterface = new BeBoardInterface(fBeBoardFWMap);
        fCbcInterface = new CbcInterface(fBeBoardFWMap);
    }

    void SystemController::ConfigureHw()
    {
        uint32_t cMissedBoard, cMissedModule, cMissedCbc;

        for(uint32_t cSId=0; cSId<fShelveVec.size(); cSId++)
        {
            cMissedBoard = 0;

            for(uint32_t cNBe=0; cNBe<fShelveVec[cSId]->getNBoard(); cNBe++)
            {
                if(fShelveVec[cSId]->getBoard(cNBe+cMissedBoard) == NULL)
                {
                    cNBe--;
                    cMissedBoard++;
                }

                else
                {
                    cMissedModule = 0;

                    fBeBoardInterface->ConfigureBoard(fShelveVec[cSId]->getBoard(cNBe+cMissedBoard));

                    for(uint32_t cNFe=0; cNFe<fShelveVec[cSId]->getBoard(cNBe+cMissedBoard)->getNFe(); cNFe++)
                    {
                        if(fShelveVec[cSId]->getBoard(cNBe+cMissedBoard)->getModule(cNFe+cMissedModule) == NULL)
                        {
                            cNFe--;
                            cMissedModule++;
                        }

                        else
                        {
                            cMissedCbc = 0;

                            for(uint32_t cNCbc=0; cNCbc<fShelveVec[cSId]->getBoard(cNBe+cMissedBoard)->getModule(cNFe+cMissedModule)->getNCbc(); cNCbc++)
                            {
                                if(fShelveVec[cSId]->getBoard(cNBe+cMissedBoard)->getModule(cNFe+cMissedModule)->getCbc(cNCbc+cMissedCbc) == NULL)
                                {
                                    cNCbc--;
                                    cMissedCbc++;
                                }

                                else
                                {
                                    fCbcInterface->ConfigureCbc(fShelveVec[cSId]->getBoard(cNBe+cMissedBoard)->getModule(cNFe+cMissedModule)->getCbc(cNCbc+cMissedCbc),false);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void SystemController::Run(BeBoard* pBeBoard, uint32_t pNthAcq)
    {
        fBeBoardInterface->Start(pBeBoard);
        fBeBoardInterface->ReadData(pBeBoard, pNthAcq, true );
        fBeBoardInterface->Stop(pBeBoard, pNthAcq);
    }

    void SystemController::SendStatusMessage(QString msg)
    {
        emit notifyStatusMessage(msg);
    }
}


