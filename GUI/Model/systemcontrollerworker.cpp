#include "QDebug"
#include "Model/systemcontrollerworker.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

namespace GUI
{

    SystemControllerWorker::SystemControllerWorker(Settings &config) :
        m_Settings(config)
    {
    }

    SystemControllerWorker::~SystemControllerWorker()
    {
        qDebug() << "Destructing " << this;
    }

    void SystemControllerWorker::InitializeHw()
    {

        uint32_t cShelveId;
        uint32_t cBeId;
        uint32_t cModuleId;
        uint32_t cCbcId;
        uint32_t cFeId;
        uint32_t cFmcId;
        uint32_t cNShelve = 0;

        //qDebug()<< m_Settings.getHwDescriptionMap();
        map_HwDescription = new QVariantMap();

        *map_HwDescription = m_Settings.getHwDescriptionMap();

        QVariantMap map_ShelveId = map_HwDescription->value("ShelveId").toMap();
        //qDebug() << map_HwDescription->keys();
        //qDebug() << map_ShelveId;

        for (auto& sh_kv: map_ShelveId.keys())
        {
            qDebug() << "Entered" << sh_kv;
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
                    qDebug() << "I'm in!";
                    cBeBoardFWInterface = new GlibFWInterface("file://settings/connections_2CBC.xml",cBeId); //TODO - get rid of XML - get from JSON
                    qDebug() << "Next" << cBeId;

                    fBeBoardFWMap[cBeId] = cBeBoardFWInterface;
                    qDebug() << "Here";

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
    }

    void SystemControllerWorker::InitializeSettings( const std::string& pFilename )
    {

    }

    void SystemControllerWorker::ConfigureHw()
    {
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
                //std::cout << GREEN << "Successfully configured Board " << int( pBoard.getBeId() ) << RESET << std::endl;
            }
            void visit( Cbc& pCbc ) {
                fCbcInterface->ConfigureCbc( &pCbc );

                qDebug() << "Successfully configured Cbc " << int( pCbc.getCbcId() );
               // std::cout << GREEN <<  "Successfully configured Cbc " << int( pCbc.getCbcId() ) << RESET << std::endl;

            }
        };

        Configurator cConfigurator( fBeBoardInterface, fCbcInterface );
        accept( cConfigurator );

        qDebug() << fShelveVector.size();//.at(0)->getBoard( 0 )->getModule( 0 )->getCbc( 0 )->getCbcId() ;
    }

    void SystemControllerWorker::CreateResultDirectory( std::string pDirname )
    {


    }

    void SystemControllerWorker::InitResultFile( std::string pFilename )
    {


    }

    void SystemControllerWorker::Run( BeBoard* pBeBoard, uint32_t pNthAcq )
    {
        fBeBoardInterface->Start( pBeBoard );
        fBeBoardInterface->ReadData( pBeBoard, pNthAcq, true );
        fBeBoardInterface->Stop( pBeBoard, pNthAcq );
    }

}
