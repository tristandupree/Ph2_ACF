/*

  FileName :                     mcp.cpp
  Content :                      MCP Test Interface program, test the software
  Programmer :                   Nicolas PIERRE, Lorenzo Bidegain
  Version :                      0.3
  Date of creation :             24/06/14
  Support :                      mail to : nico.pierre@icloud.com, lorenzo.bidegain@cern.ch

*/

#include "../HWDescription/Cbc.h"
#include "../HWDescription/Module.h"
#include "../HWDescription/Glib.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/GLIBInterface.h"
#include "../HWDescription/Definition.h"
#include "../HWInterface/Utilities.h"
#include <boost/format.hpp>

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

int main()
{
    int i;

    BeBoard* cBeBoard;
    Module* cModule;
    Cbc* cCbc;
    uint32_t cBoardId, cModuleId, cCbcId, cShelveId, cBeId, cFMCId, cFeId, cNFe;
    std::string cFilePath;
    std::string cRegNode;
    std::string cValue;
    uint32_t cValueHex;
    uint32_t cNthAcq;
    bool cBreakTrigger;
    int cMissedModule = 0;
    int cMissedCbc = 0;

    BeBoardFWMap cBeBoardFWMap;
    std::vector<BeBoard*> cBeBoardVec;

    BeBoardFWInterface* cBeBoardFWInterface;
    BeBoardInterface cBeBoardInterface(cBeBoardFWMap);
    CbcInterface cCbcInterface(cBeBoardFWMap);

    std::cout << "\n\n\n\n" << std::endl;
    std::cout << "****************************************************************" << std::endl;
    std::cout << "****************************************************************" << std::endl;
    std::cout << "***          MCP Test Program for Ph2 Base Software          ***" << std::endl;
    std::cout << "***                N. Pierre, L. Bidegain v0.3               ***" << std::endl;
    std::cout << "****************************************************************" << std::endl;
    std::cout << "****************************************************************" << std::endl;

    do
    {

        std::cout << "\n\n\n\n" << std::endl;
        std::cout << "****************************************************" << std::endl;
        std::cout << "***                  Main Menu                   ***" << std::endl;
        std::cout << "****************************************************\n" << std::endl;
        std::cout << "\n\n\n\nWhat do you want to do ?\n" << std::endl;
        std::cout << "1: Add/Remove Board/Module/Cbc" << std::endl;
        std::cout << "2: Configure" << std::endl;
        std::cout << "3: Glib Manipulation" << std::endl;
        std::cout << "4: Cbc Manipulation" << std::endl;
        std::cout << "5: Acquisition-ish" << std::endl;
        std::cout << "6: Configuration Recap" << std::endl;
        std::cout << "0: Quit\n" << std::endl;

        std::cin >> i;

        switch(i)
        {

            case 1:

                std::cout << "\n\n\n\n"<< std::endl;
                std::cout << "****************************************************" << std::endl;
                std::cout << "***            Add a Module or a Cbc             ***" << std::endl;
                std::cout << "****************************************************\n" << std::endl;

                std::cout << "1: Add Board" << std::endl;
                std::cout << "2: Add Module" << std::endl;
                std::cout << "3: Add Cbc" << std::endl;
                std::cout << "4: Remove Board" << std::endl;
                std::cout << "5: Remove Module" << std::endl;
                std::cout << "6: Remove Cbc\n" << std::endl;

                std::cin >> i;

                switch(i)
                {

                    case 1:
                        std::cout << "*** Add Board ***" << std::endl;
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;
                        std::cout << "--> Which ShelveId ?" << std::endl;
                        std::cin >> cShelveId;
                        std::cout << "--> How many FE ?" << std::endl;
                        std::cin >> cNFe;

                        if(cBeBoardFWMap.find(cBoardId) == cBeBoardFWMap.end())
                        {
                            cBeBoard->fBeId=cBoardId;
                            cBeBoardFWInterface = new BeBoardFWInterface(UHAL_CONFIG_FILE,cBoardId);
                            if(cBeBoardFWInterface->getBoardType() == "GLIB")
                            {
                                delete cBeBoardFWInterface;
                                cBeBoardFWMap[cBoardId] = new GlibFWInterface(UHAL_CONFIG_FILE,cBoardId);
                                cBeBoardVec.push_back(new BeBoard(cShelveId,cBoardId,cNFe));
                                std::cout << "*** Glib Added ***" << std::endl;
                            }
                            else
                            {
                                delete cBeBoardFWInterface;
                                std::cout << "ERROR : Unknown type of board !" << std::endl;
                            }
                        }
                        else
                        {
                            std::cout << "ERROR : This board already exists !" << std::endl;
                            myflush( std::cin );
                            mypause();
                        }
                    break;


                    case 2:
                        std::cout << "*** Add Module ***" << std::endl;
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                        {
                            if(cBeBoardVec[j]->fBeId == cBoardId)
                            {
                                std::cout << "--> Which ModuleId ?" << std::endl;
                                std::cin >> cModuleId;

                                if(cBeBoardVec[j]->getModule(cModuleId) == NULL)
                                {
                                    std::cout << "--> Which FMCId ?" << std::endl;
                                    std::cin >> cFMCId;
                                    std::cout << "--> Which FeId ?" << std::endl;
                                    std::cin >> cFeId;

                                    cModule.fModuleId=cModuleId;
                                    cModule.fFMCId=cFMCId;
                                    cModule.fFeId=cFeId;
                                    cModule.fBeId=cBeBoardVec[j]->fBeId;
                                    cModule.fShelveId=cBeBoardVec[j]->fShelveId;

                                    cBeBoardFWMap[cBoardId]->addModule(cModule);
                                    std::cout << "*** Module Added ***" << std::endl;
                                }
                                else
                                {
                                    std::cout << "ERROR : This module already exists !" << std::endl;
                                    myflush( std::cin );
                                    mypause();
                                }
                            }
                            else if(j == cBeBoardVec.size())
                            {
                                std::cout << "ERROR : This board does not exist !" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                        }
                    break;


                    case 3:
                        std::cout << "*** Add Cbc ***" << std::endl;
                        std::cout << "1: Add Default Cbc" << std::endl;
                        std::cout << "2: Add Personalised Cbc" << std::endl;
                        std::cout << "3: Add a 2Cbc Hybrid Structure\n" << std::endl;
                        std::cout << "4: Add a 8Cbc Hybrid Structure\n" << std::endl;

                        std::cin >> i;

                        switch(i)
                        {

                            case 1:
                                std::cout << "*** Add Default Cbc ***" << std::endl;
                                std::cout << "--> Which BoardId ?" << std::endl;
                                std::cin >> cBoardId;

                                for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                                {

                                    if(cBeBoardVec[j]->fBeId == cBoardId)
                                    {
                                        std::cout << "--> Which ModuleId ?" << std::endl;
                                        std::cin >> cModuleId;
                                        if (cBeBoardVec[j]->getModule(cModuleId) == NULL)
                                        {
                                            std::cout << "*** ERROR !!                                      ***" << std::endl;
                                            std::cout << "*** This module does not exist !                  ***" << std::endl;
                                            std::cout << "*** This is not the module you are looking for... ***" << std::endl;
                                            myflush( std::cin );
                                            mypause();
                                        }
                                        else
                                        {
                                            std::cout << "--> Which CbcId ?" << std::endl;
                                            std::cin >> cCbcId;
                                            if(cBeBoardVec[j]->getModule(cModuleId)->getCbc(cCbcId) == NULL)
                                            {
                                                cCbc = new Cbc(0,0,0,0,cCbcId,DEFAULT_FILE);
                                                cBeBoardVec[j]->getModule(cModuleId)->addCbc(*cCbc);
                                                delete cCbc;
                                                std::cout << "*** Cbc Added ***" << std::endl;
                                            }
                                            else
                                            {
                                                std::cout << "ERROR : This cbc already exists !" << std::endl;
                                                myflush( std::cin );
                                                mypause();
                                            }
                                        }
                                    }
                                    else if(j == cBeBoardVec.size())
                                    {
                                        std::cout << "ERROR : This board does not exist !" << std::endl;
                                        myflush( std::cin );
                                        mypause();
                                    }
                                }
                            break;


                            case 2:
                                std::cout << "*** Add Personalised Cbc ***" << std::endl;
                                std::cout << "--> Which BoardId ?" << std::endl;
                                std::cin >> cBoardId;
                                for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                                {

                                    if(cBeBoardVec[j]->fBeId == cBoardId)
                                    {
                                        std::cout << "--> Which ModuleId ?" << std::endl;
                                        std::cin >> cModuleId;
                                        if (cBeBoardVec[j]->getModule(cModuleId) == NULL)
                                        {
                                            std::cout << "*** ERROR !!                                      ***" << std::endl;
                                            std::cout << "*** This module does not exist !                  ***" << std::endl;
                                            std::cout << "*** This is not the module you are looking for... ***" << std::endl;
                                            myflush( std::cin );
                                            mypause();
                                        }
                                        else
                                        {
                                            std::cout << "--> Which CbcId ?" << std::endl;
                                            std::cin >> cCbcId;

                                            if(cBeBoardVec[j]->getModule(cModuleId)->getCbc(cCbcId) == NULL)
                                            {

                                                std::cout << "--> Which ShelveId ?" << std::endl;
                                                std::cout << "(Put 0 if you want it per default)" << std::endl;
                                                std::cin >> cShelveId;

                                                std::cout << "--> Which BeId ?" << std::endl;
                                                std::cout << "(Put 0 if you want it per default)" << std::endl;
                                                std::cin >> cBeId;

                                                std::cout << "--> Which FMCId ?" << std::endl;
                                                std::cout << "(Put 0 if you want it per default)" << std::endl;
                                                std::cin >> cFMCId;

                                                std::cout << "--> Which FeId ?" << std::endl;
                                                std::cout << "(Put 0 if you want it per default)" << std::endl;
                                                std::cin >> cFeId;

                                                std::cout << "--> Choose your Config File" << std::endl;
                                                std::cout << "1: FE0CBC0HOLE" << std::endl;
                                                std::cout << "2: FE0CBC1" << std::endl;
                                                std::cout << "3: FE0CBC1HOLE" << std::endl;
                                                std::cout << "4: Your own Config File" << std::endl;
                                                std::cin >> i;

                                                switch(i)
                                                {

                                                    case 1:
                                                        {
                                                            Cbc cCbcPers(cShelveId,cBeId,cFMCId,cFeId,cCbcId,FE0CBC0HOLE);
                                                            cBeBoardVec[j]->getModule(cModuleId)->addCbc(cCbcPers);
                                                            std::cout << "*** Cbc Added ***" << std::endl;
                                                        }
                                                    break;


                                                    case 2:
                                                        {
                                                            Cbc cCbcPers(cShelveId,cBeId,cFMCId,cFeId,cCbcId,FE0CBC1);
                                                            cBeBoardVec[j]->getModule(cModuleId)->addCbc(cCbcPers);
                                                            std::cout << "*** Cbc Added ***" << std::endl;
                                                        }
                                                    break;


                                                    case 3:
                                                        {
                                                            Cbc cCbcPers(cShelveId,cBeId,cFMCId,cFeId,cCbcId,FE0CBC1HOLE);
                                                            cBeBoardVec[j]->getModule(cModuleId)->addCbc(cCbcPers);
                                                            std::cout << "*** Cbc Added ***" << std::endl;
                                                        }
                                                    break;


                                                    case 4:
                                                        {
                                                            std::cout << "--> Enter your File Path " << std::endl;
                                                            std::cout << "(Absolute path please) " << std::endl;
                                                            std::cin >> cFilePath;
                                                            Cbc cCbcPers(cShelveId,cBeId,cFMCId,cFeId,cCbcId,cFilePath);
                                                            cBeBoardVec[j]->getModule(cModuleId)->addCbc(cCbcPers);
                                                            std::cout << "*** Cbc Added ***" << std::endl;
                                                        }
                                                    break;


                                                    default:
                                                        std::cout << "*** This is not the option you are looking for... ***" << std::endl;
                                                        myflush( std::cin );
                                                        mypause();
                                                    break;

                                                }
                                            }
                                            else
                                            {
                                                std::cout << "ERROR : This cbc already exists !" << std::endl;
                                            }
                                        }
                                    }
                                    else if(j == cBeBoardVec.size())
                                    {
                                        std::cout << "ERROR : This board does not exist !" << std::endl;
                                        myflush( std::cin );
                                        mypause();
                                    }
                                }
                            break;


                            case 3:
                                std::cout << "*** Add a 2Cbc Hybrid Structure ***" << std::endl;
                                std::cout << "--> Which BoardId ?" << std::endl;
                                std::cin >> cBoardId;

                                for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                                {

                                    if(cBeBoardVec[j]->fBeId == cBoardId)
                                    {
                                        std::cout << "--> Which ModuleId ?" << std::endl;
                                        std::cin >> cModuleId;
                                        if (cBeBoardVec[j]->getModule(cModuleId) == NULL)
                                        {
                                            std::cout << "*** ERROR !!                                      ***" << std::endl;
                                            std::cout << "*** This module does not exist !                  ***" << std::endl;
                                            std::cout << "*** This is not the module you are looking for... ***" << std::endl;
                                            myflush( std::cin );
                                            mypause();
                                        }
                                        else
                                        {
                                            for(uint8_t i=0; i<2; i++)
                                            {
                                                if(cBeBoardVec[j]->getModule(cModuleId)->getCbc(i) == NULL)
                                                {
                                                    cCbc = new Cbc(0,0,0,0,i,(boost::format("settings/FE0CBC%d.txt") %(uint32_t(i))).str());
                                                    cBeBoardVec[j]->getModule(cModuleId)->addCbc(*cCbc);
                                                    delete cCbc;
                                                }
                                            }

                                            cCbcInterface.ReadAllCbc(cBeBoardVec[j]->getModule(cModuleId));

                                            std::cout << "*** 2Cbc Hybrid Structure Added ***" << std::endl;
                                        }
                                    }
                                    else if(j == cBeBoardVec.size())
                                    {
                                        std::cout << "ERROR : This board does not exist !" << std::endl;
                                        myflush( std::cin );
                                        mypause();
                                    }
                                }
                            break;



                            case 4:
                                std::cout << "*** Add a 8Cbc Hybrid Structure ***" << std::endl;
                                std::cout << "--> Which BoardId ?" << std::endl;
                                std::cin >> cBoardId;

                                for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                                {

                                    if(cBeBoardVec[j]->fBeId == cBoardId)
                                    {
                                        std::cout << "--> Which ModuleId ?" << std::endl;
                                        std::cin >> cModuleId;
                                        if (cBeBoardVec[j]->getModule(cModuleId) == NULL)
                                        {
                                            std::cout << "*** ERROR !!                                      ***" << std::endl;
                                            std::cout << "*** This module does not exist !                  ***" << std::endl;
                                            std::cout << "*** This is not the module you are looking for... ***" << std::endl;
                                            myflush( std::cin );
                                            mypause();
                                        }
                                        else
                                        {
                                            for(uint8_t i=0; i<8; i++)
                                            {
                                                if(cBeBoardVec[j]->getModule(cModuleId)->getCbc(i) == NULL)
                                                {
                                                    cCbc = new Cbc(0,0,0,0,i,(boost::format("settings/FE0CBC%d.txt") %(uint32_t(i))).str());
                                                    cBeBoardVec[j]->getModule(cModuleId)->addCbc(*cCbc);
                                                    delete cCbc;
                                                }
                                            }

                                            cCbcInterface.UpdateAllCbc(cBeBoardVec[j]->getModule(cModuleId));

                                            std::cout << "*** 8Cbc Hybrid Structure Added ***" << std::endl;
                                        }
                                    }
                                    else if(j == cBeBoardVec.size())
                                    {
                                        std::cout << "ERROR : This board does not exist !" << std::endl;
                                        myflush( std::cin );
                                        mypause();
                                    }
                                }
                            break;


                            default:
                                std::cout << "*** This is not the option you are looking for... ***" << std::endl;
                            break;

                        }
                    break;


                    case 4:
                        std::cout << "*** Remove Board ***" << std::endl;
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        std::vector<BeBoard*>::iterator cIt;

                        for(cIt=cBeBoardVec.begin(); cIt!=cBeBoardVec.end(); cIt++)
                        {
                            if((*cIt)->fBeId == cBoardId)
                            {
                                cBeBoardFWMap.erase(cBoardId);
                                cBeBoardVec.erase(cIt);
                                std::cout << "*** Board removed ! ***" << std::endl;
                            }
                        }

                        if(cIt == cBeBoardVec.end())
                        {
                            std::cout << "ERROR : This board does not exist !" << std::endl;
                            myflush( std::cin );
                            mypause();
                        }

                    break;


                    case 5:
                        std::cout << "*** Remove Module ***" << std::endl;
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                        {

                            if(cBeBoardVec[j]->fBeId == cBoardId)
                            {
                                std::cout << "--> Which ModuleId ?" << std::endl;
                                std::cin >> cModuleId;

                                if (cBeBoardVec[j]->getModule(cModuleId) == NULL)
                                {
                                    std::cout << "*** ERROR !!                                      ***" << std::endl;
                                    std::cout << "*** This module does not exist !                  ***" << std::endl;
                                    std::cout << "*** This is not the module you are looking for... ***" << std::endl;
                                }
                                else
                                {
                                    if(cBeBoardVec[j]->removeModule(cModuleId))
                                        std::cout << "*** Module removed ! ***" << std::endl;
                                    else
                                        std::cout << "*** ERROR in Module removal !! ***" << std::endl;
                                }
                            }
                            else if(j == cBeBoardVec.size())
                            {
                                std::cout << "ERROR : This board does not exist !" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                        }
                    break;


                    case 6:
                        std::cout << "*** Remove Cbc ***" << std::endl;
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                        {

                            if(cBeBoardVec[j]->fBeId == cBoardId)
                            {
                                std::cout << "--> Which ModuleId ?" << std::endl;
                                std::cin >> cModuleId;

                                if(cBeBoardVec[j]->getModule(cModuleId) == NULL)
                                {
                                    std::cout << "*** ERROR !!                                      ***" << std::endl;
                                    std::cout << "*** This module does not exist !                  ***" << std::endl;
                                    std::cout << "*** This is not the module you are looking for... ***" << std::endl;
                                    myflush( std::cin );
                                    mypause();
                                }
                                else
                                {
                                    std::cout << "--> Which CbcId ?" << std::endl;
                                    std::cin >> cCbcId;
                                    if(cBeBoardVec[j]->getModule(cModuleId)->getCbc(cCbcId) == NULL)
                                    {
                                        std::cout << "*** ERROR !!                                      ***" << std::endl;
                                        std::cout << "*** This Cbc does not exist !                     ***" << std::endl;
                                        std::cout << "*** This is not the Cbc you are looking for...    ***" << std::endl;
                                        myflush( std::cin );
                                        mypause();
                                    }
                                    else
                                    {
                                        if(cBeBoardVec[j]->getModule(cModuleId)->removeCbc(cCbcId))
                                            std::cout << "*** Module removed ! ***" << std::endl;
                                        else
                                            std::cout << "*** ERROR in Cbc removal !! ***" << std::endl;
                                    }
                                    std::cout << "*** Cbc Removed ***" << std::endl;
                                }
                            }
                            else if(j == cBeBoardVec.size())
                            {
                                std::cout << "ERROR : This board does not exist !" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                        }
                    break;


                    default:
                        std::cout << "*** This is not the option you are looking for... ***" << std::endl;
                        myflush( std::cin );
                        mypause();
                    break;

                }

            break;


            case 2:

                std::cout << "\n\n\n\n"<< std::endl;
                std::cout << "****************************************************" << std::endl;
                std::cout << "***                  Configure                   ***" << std::endl;
                std::cout << "****************************************************\n" << std::endl;

                std::cout << "1: Configure Board" << std::endl;
                std::cout << "2: Configure Cbc" << std::endl;
                std::cout << "3: Configure all Cbcs in one Module" << std::endl;
                std::cout << "4: Configure all Cbcs in all Modules\n" << std::endl;

                std::cin >> i;

                switch(i)
                {

                    case 1:
                        std::cout << "*** Configure Board ***" << std::endl;
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                        {

                            if(cBeBoardVec[j]->fBeId == cBoardId)
                            {
                                cBeBoardInterface.ConfigureBoard(cBeBoardVec[j]);
                                std::cout << "*** Glib Configured ***" << std::endl;
                            }
                            else if(j == cBeBoardVec.size())
                            {
                                std::cout << "ERROR : This board does not exist !" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                        }
                    break;


                    case 2:
                        std::cout << "*** Configure Cbc ***" << std::endl;
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                        {

                            if(cBeBoardVec[j]->fBeId == cBoardId)
                            {
                                std::cout << "--> Which ModuleId ?" << std::endl;
                                std::cin >> cModuleId;
                                if(cBeBoardVec[j]->getModule(cModuleId) == NULL)
                                {
                                    std::cout << "*** ERROR !!                                      ***" << std::endl;
                                    std::cout << "*** This module does not exist !                  ***" << std::endl;
                                    std::cout << "*** This is not the module you are looking for... ***" << std::endl;
                                    myflush( std::cin );
                                    mypause();
                                }
                                else
                                {
                                    std::cout << "--> Which CbcId ?" << std::endl;
                                    std::cin >> cCbcId;
                                    if(cBeBoardVec[j]->getModule(cModuleId)->getCbc(cCbcId) == NULL)
                                    {
                                        std::cout << "*** ERROR !!                                      ***" << std::endl;
                                        std::cout << "*** This Cbc does not exist !                     ***" << std::endl;
                                        std::cout << "*** This is not the Cbc you are looking for...    ***" << std::endl;
                                        myflush( std::cin );
                                        mypause();
                                    }
                                    else
                                    {
                                        cCbcInterface.ConfigureCbc(cBeBoardVec[j]->getModule(cModuleId)->getCbc(cCbcId));
                                    }
                                    std::cout << "*** Cbc Configured ***" << std::endl;
                                }
                            }
                            else if(j == cBeBoardVec.size())
                            {
                                std::cout << "ERROR : This board does not exist !" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                        }
                    break;


                    case 3:
                        std::cout << "*** Configure all Cbcs in one Module***" << std::endl;
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                        {

                            if(cBeBoardVec[j]->fBeId == cBoardId)
                            {
                                std::cout << "--> Which ModuleId ?" << std::endl;
                                std::cin >> cModuleId;

                                if(cBeBoardVec[j]->getModule(cModuleId) == NULL)
                                {
                                    std::cout << "*** ERROR !!                                      ***" << std::endl;
                                    std::cout << "*** This module does not exist !                  ***" << std::endl;
                                    std::cout << "*** This is not the module you are looking for... ***" << std::endl;
                                    myflush( std::cin );
                                    mypause();
                                }
                                else
                                {
                                    for(uint8_t j=0;j<cBeBoardVec[j]->getModule(cModuleId)->getNCbc();j++)
                                    {
                                        if(cBeBoardVec[j]->getModule(cModuleId)->getCbc(j+cMissedCbc) == NULL)
                                        {
                                            j--;
                                            cMissedCbc++;
                                        }

                                        else
                                        {
                                            cCbcInterface.ConfigureCbc(cBeBoardVec[j]->getModule(cModuleId)->getCbc(j+cMissedCbc));
                                        }
                                    }
                                    cMissedCbc = 0;
                                    std::cout << "*** All Cbcs in the Module Configured ***" << std::endl;
                                }
                            }
                            else if(j == cBeBoardVec.size())
                            {
                                std::cout << "ERROR : This board does not exist !" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                        }
                    break;


                    case 4:
                        std::cout << "*** Configure all Cbcs in all Modules ***" << std::endl;
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                        {

                            if(cBeBoardVec[j]->fBeId == cBoardId)
                            {
                                for(uint8_t k=0;k<cBeBoardVec[j]->getNFe();k++)
                                {
                                    if(cBeBoardVec[j]->getModule(k+cMissedModule) == NULL)
                                    {
                                        k--;
                                        cMissedModule++;
                                    }

                                    else
                                    {
                                        for(uint8_t j=0;j<cBeBoardVec[j]->getModule(k+cMissedModule)->getNCbc();j++)
                                        {
                                            if(cBeBoardVec[j]->getModule(k+cMissedModule)->getCbc(j+cMissedCbc) == NULL)
                                            {
                                                j--;
                                                cMissedCbc++;
                                            }

                                            else
                                            {
                                                cCbcInterface.ConfigureCbc(cBeBoardVec[j]->getModule(k+cMissedModule)->getCbc(j+cMissedCbc));
                                            }
                                        }
                                        cMissedCbc = 0;
                                    }
                                }
                                cMissedModule = 0;
                                std::cout << "*** All Cbcs in all Modules Configured ***" << std::endl;
                            }
                            else if(j == cBeBoardVec.size())
                            {
                                std::cout << "ERROR : This board does not exist !" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                        }
                    break;


                    default:
                        std::cout << "*** This is not the option you are looking for... ***" << std::endl;
                        myflush( std::cin );
                        mypause();
                    break;

                }

            break;


            case 3:

                std::cout << "\n\n\n\n"<< std::endl;
                std::cout << "****************************************************" << std::endl;
                std::cout << "***              Board Manipulation              ***" << std::endl;
                std::cout << "****************************************************\n" << std::endl;

                std::cout << "1: Update both ways" << std::endl;
                std::cout << "2: Update one way" << std::endl;
                std::cout << "3: Get infos\n" << std::endl;

                std::cin >> i;

                switch(i)
                {

                    case 1:
                        std::cout << "*** Update both ways ***" << std::endl;
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                        {

                            if(cBeBoardVec[j]->fBeId == cBoardId)
                            {
                                std::cout << "--> Which Register ?" << std::endl;
                                std::cin >> cRegNode;
                                std::cout << "--> Which Value ? (0x__)" << std::endl;
                                std::cin >> cValue;
                                cValueHex = strtoul( cValue.c_str(), 0, 16 );
                                if(cValueHex > 255)
                                {
                                    std::cout << "*** ERROR !!                                      ***" << std::endl;
                                    std::cout << "*** This value exceed the maximum value (OxFF) !  ***" << std::endl;
                                    myflush( std::cin );
                                    mypause();
                                }
                                else
                                {
                                    cBeBoardInterface.WriteBoardReg(cBeBoardVec[j],cRegNode,cValueHex);
                                    std::cout << "*** Updated ***" << std::endl;
                                }
                            }
                            else if(j == cBeBoardVec.size())
                            {
                                std::cout << "ERROR : This board does not exist !" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                        }
                    break;


                    case 2:
                        std::cout << "*** Update both ways ***" << std::endl;
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                        {

                            if(cBeBoardVec[j]->fBeId == cBoardId)
                            {
                                std::cout << "*** Update one way ***" << std::endl;
                                std::cout << "--> Which Register ?" << std::endl;
                                std::cin >> cRegNode;
                                cBeBoardInterface.ReadBoardReg(cBeBoardVec[j],cRegNode);
                                std::cout << "*** Updated ***" << std::endl;
                            }
                            else if(j == cBeBoardVec.size())
                            {
                                std::cout << "ERROR : This board does not exist !" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                        }
                    break;


                    case 3:
                        std::cout << "*** Get Infos ***" << std::endl;
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                        {

                            if(cBeBoardVec[j]->fBeId == cBoardId)
                            {
                                cBeBoardInterface.getBoardInfo(cBeBoardVec[j]);
                                std::cout << "*** Infos Get ***" << std::endl;
                            }
                            else if(j == cBeBoardVec.size())
                            {
                                std::cout << "ERROR : This board does not exist !" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                        }
                    break;


                    default:
                        std::cout << "*** This is not the option you are looking for... ***" << std::endl;
                        myflush( std::cin );
                        mypause();
                    break;

                }

            break;


            case 4:

                std::cout << "\n\n\n\n"<< std::endl;
                std::cout << "****************************************************" << std::endl;
                std::cout << "***               Cbc Manipulation               ***" << std::endl;
                std::cout << "****************************************************\n" << std::endl;

                std::cout << "1: Write Cbc" << std::endl;
                std::cout << "2: Update Cbc" << std::endl;
                std::cout << "3: Write all Cbc" << std::endl;
                std::cout << "4: Update all Cbc" << std::endl;
                std::cout << "5: Hard Reset a Cbc" << std::endl;
                std::cout << "6: Fast Reset Cbc\n" << std::endl;

                std::cin >> i;

                switch(i)
                {

                    case 1:
                        std::cout << "*** Write Cbc ***" << std::endl;
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                        {

                            if(cBeBoardVec[j]->fBeId == cBoardId)
                            {
                                std::cout << "--> Which ModuleId ?" << std::endl;
                                std::cin >> cModuleId;
                                if(cBeBoardVec[j]->getModule(cModuleId) == NULL)
                                {
                                    std::cout << "*** ERROR !!                                      ***" << std::endl;
                                    std::cout << "*** This module does not exist !                  ***" << std::endl;
                                    std::cout << "*** This is not the module you are looking for... ***" << std::endl;
                                    myflush( std::cin );
                                    mypause();
                                }
                                else
                                {
                                    std::cout << "--> Which CbcId ?" << std::endl;
                                    std::cin >> cCbcId;
                                    if(cBeBoardVec[j]->getModule(cModuleId)->getCbc(cCbcId) == NULL)
                                    {
                                        std::cout << "*** ERROR !!                                      ***" << std::endl;
                                        std::cout << "*** This Cbc does not exist !                     ***" << std::endl;
                                        std::cout << "*** This is not the Cbc you are looking for...    ***" << std::endl;
                                        myflush( std::cin );
                                        mypause();
                                    }
                                    else
                                    {
                                        std::cout << "--> Which Register ?" << std::endl;
                                        std::cin >> cRegNode;
                                        std::cout << "--> Which Value ? (0x__)" << std::endl;
                                        std::cin >> cValue;
                                        cValueHex = strtoul( cValue.c_str(), 0, 16 );
                                        if(cValueHex > 255)
                                        {
                                            std::cout << "*** ERROR !!                                      ***" << std::endl;
                                            std::cout << "*** This value exceed the maximum value (OxFF) !  ***" << std::endl;
                                            myflush( std::cin );
                                            mypause();
                                        }
                                        else
                                        {
                                            cCbcInterface.WriteCbcReg(cBeBoardVec[j]->getModule(cModuleId)->getCbc(cCbcId),cRegNode,cValueHex);
                                            std::cout << "*** Cbc Written ***" << std::endl;
                                        }
                                    }
                                }
                            }
                            else if(j == cBeBoardVec.size())
                            {
                                std::cout << "ERROR : This board does not exist !" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                        }
                    break;


                    case 2:
                        std::cout << "*** Update Cbc ***" << std::endl;
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                        {

                            if(cBeBoardVec[j]->fBeId == cBoardId)
                            {
                                std::cout << "--> Which ModuleId ?" << std::endl;
                                std::cin >> cModuleId;
                                if(cBeBoardVec[j]->getModule(cModuleId) == NULL)
                                {
                                    std::cout << "*** ERROR !!                                      ***" << std::endl;
                                    std::cout << "*** This module does not exist !                  ***" << std::endl;
                                    std::cout << "*** This is not the module you are looking for... ***" << std::endl;
                                    myflush( std::cin );
                                    mypause();
                                }
                                else
                                {
                                    std::cout << "--> Which CbcId ?" << std::endl;
                                    std::cin >> cCbcId;
                                    if(cBeBoardVec[j]->getModule(cModuleId)->getCbc(cCbcId) == NULL)
                                    {
                                        std::cout << "*** ERROR !!                                      ***" << std::endl;
                                        std::cout << "*** This Cbc does not exist !                     ***" << std::endl;
                                        std::cout << "*** This is not the Cbc you are looking for...    ***" << std::endl;
                                        myflush( std::cin );
                                        mypause();
                                    }
                                    else
                                    {
                                        std::cout << "--> Which Register ?" << std::endl;
                                        std::cin >> cRegNode;
                                        cCbcInterface.ReadCbcReg(cBeBoardVec[j]->getModule(cModuleId)->getCbc(cCbcId),cRegNode);
                                        std::cout << "*** Updated ***" << std::endl;
                                    }
                                }
                            }
                            else if(j == cBeBoardVec.size())
                            {
                                std::cout << "ERROR : This board does not exist !" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                        }
                    break;


                    case 3:
                        std::cout << "*** Write all Cbc ***" << std::endl;
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                        {

                            if(cBeBoardVec[j]->fBeId == cBoardId)
                            {
                                std::cout << "--> Which ModuleId ?" << std::endl;
                                std::cin >> cModuleId;
                                if(cBeBoardVec[j]->getModule(cModuleId) == NULL)
                                {
                                    std::cout << "*** ERROR !!                                      ***" << std::endl;
                                    std::cout << "*** This module does not exist !                  ***" << std::endl;
                                    std::cout << "*** This is not the module you are looking for... ***" << std::endl;
                                    myflush( std::cin );
                                    mypause();
                                }
                                else
                                {
                                    std::cout << "--> Which Register ?" << std::endl;
                                    std::cin >> cRegNode;
                                    std::cout << "--> Which Value ? (0x__)" << std::endl;
                                    std::cin >> cValue;
                                    cValueHex = strtoul( cValue.c_str(), 0, 16 );
                                    if(cValueHex > 255)
                                    {
                                        std::cout << "*** ERROR !!                                      ***" << std::endl;
                                        std::cout << "*** This value exceed the maximum value (OxFF) !  ***" << std::endl;
                                        myflush( std::cin );
                                        mypause();
                                    }
                                    else
                                    {
                                        cCbcInterface.WriteBroadcast(cBeBoardVec[j]->getModule(cModuleId),cRegNode,cValueHex);
                                        std::cout << "*** All Cbc Written ***" << std::endl;
                                    }
                                }
                            }
                            else if(j == cBeBoardVec.size())
                            {
                                std::cout << "ERROR : This board does not exist !" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                        }
                    break;


                    case 4:
                        std::cout << "*** Read all Cbc ***" << std::endl;
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                        {

                            if(cBeBoardVec[j]->fBeId == cBoardId)
                            {
                                std::cout << "--> Which ModuleId ?" << std::endl;
                                std::cin >> cModuleId;
                                if(cBeBoardVec[j]->getModule(cModuleId) == NULL)
                                {
                                    std::cout << "*** ERROR !!                                      ***" << std::endl;
                                    std::cout << "*** This module does not exist !                  ***" << std::endl;
                                    std::cout << "*** This is not the module you are looking for... ***" << std::endl;
                                    myflush( std::cin );
                                    mypause();
                                }
                                else
                                {
                                    cCbcInterface.ReadAllCbc(cBeBoardVec[j]->getModule(cModuleId));
                                    std::cout << "*** All Cbc Updated ***" << std::endl;
                                }
                            }
                            else if(j == cBeBoardVec.size())
                            {
                                std::cout << "ERROR : This board does not exist !" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                        }
                    break;


                    case 5:
                        std::cout << "*** Cbc Hard Reset ***" << std::endl;
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                        {

                            if(cBeBoardVec[j]->fBeId == cBoardId)
                            {
                                std::cout << "--> Which ModuleId ?" << std::endl;
                                std::cin >> cModuleId;
                                if(cBeBoardVec[j]->getModule(cModuleId) == NULL)
                                {
                                    std::cout << "*** ERROR !!                                      ***" << std::endl;
                                    std::cout << "*** This module does not exist !                  ***" << std::endl;
                                    std::cout << "*** This is not the module you are looking for... ***" << std::endl;
                                    myflush( std::cin );
                                    mypause();
                                }
                                else
                                {
                                    std::cout << "--> Which CbcId ?" << std::endl;
                                    std::cin >> cCbcId;
                                    if(cBeBoardVec[j]->getModule(cModuleId)->getCbc(cCbcId) == NULL)
                                    {
                                        std::cout << "*** ERROR !!                                      ***" << std::endl;
                                        std::cout << "*** This Cbc does not exist !                     ***" << std::endl;
                                        std::cout << "*** This is not the Cbc you are looking for...    ***" << std::endl;
                                        myflush( std::cin );
                                        mypause();
                                    }
                                    else
                                    {
                                        cCbcInterface.CbcHardReset(cBeBoardVec[j]->getModule(cModuleId)->getCbc(cCbcId));
                                    }
                                std::cout << "*** Cbc Hard Reset ***" << std::endl;
                                }
                            }
                            else if(j == cBeBoardVec.size())
                            {
                                std::cout << "ERROR : This board does not exist !" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                        }
                    break;


                    case 6:
                        std::cout << "*** Cbc Fast Reset ***" << std::endl;
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                        {

                            if(cBeBoardVec[j]->fBeId == cBoardId)
                            {
                                std::cout << "--> Which ModuleId ?" << std::endl;
                                std::cin >> cModuleId;
                                if(cBeBoardVec[j]->getModule(cModuleId) == NULL)
                                {
                                    std::cout << "*** ERROR !!                                      ***" << std::endl;
                                    std::cout << "*** This module does not exist !                  ***" << std::endl;
                                    std::cout << "*** This is not the module you are looking for... ***" << std::endl;
                                    myflush( std::cin );
                                    mypause();
                                }
                                else
                                {
                                    std::cout << "--> Which CbcId ?" << std::endl;
                                    std::cin >> cCbcId;
                                    if(cBeBoardVec[j]->getModule(cModuleId)->getCbc(cCbcId) == NULL)
                                    {
                                        std::cout << "*** ERROR !!                                      ***" << std::endl;
                                        std::cout << "*** This Cbc does not exist !                     ***" << std::endl;
                                        std::cout << "*** This is not the Cbc you are looking for...    ***" << std::endl;
                                        myflush( std::cin );
                                        mypause();
                                    }
                                    else
                                    {
                                        cCbcInterface.CbcFastReset(cBeBoardVec[j]->getModule(cModuleId)->getCbc(cCbcId));
                                    }
                                std::cout << "*** Cbc Fast Reset ***" << std::endl;
                                }
                            }
                            else if(j == cBeBoardVec.size())
                            {
                                std::cout << "ERROR : This board does not exist !" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                        }
                    break;


                    default:
                        std::cout << "*** This is not the option you are looking for... ***" << std::endl;
                        myflush( std::cin );
                        mypause();
                    break;

                }

            break;


            case 5:

                std::cout << "\n\n\n\n"<< std::endl;
                std::cout << "****************************************************" << std::endl;
                std::cout << "***               Acquisition-ish                ***" << std::endl;
                std::cout << "****************************************************\n" << std::endl;

                std::cout << "1: Read Data\n" << std::endl;

                std::cin >> i;

                switch(i)
                {

                    case 1:
                        std::cout << "*** Run Acquisition ***" << std::endl;
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                        {

                            if(cBeBoardVec[j]->fBeId == cBoardId)
                            {
                                std::ofstream cfile( "output/TestData.dat", std::ios::out | std::ios::trunc );

                                uint32_t cNthAcq = 0;
                                uint32_t cNevents = 10;
                                uint32_t cN = 0;

                                TCanvas *cCanvas = new TCanvas("Data Acq", "Different hits counters", 600, 400);
                                TCanvas *cCanvasMean = new TCanvas("Data Acq Mean", "Different hits counters", 600, 400);
                                TH1F *cHist = NULL;
                                TH1F *cHistMean = new TH1F("Histo_Hits Mean", "Hit Counter", uint32_t(pGlib.getModule(0)->getNCbc()), 0., uint32_t(pGlib.getModule(0)->getNCbc()));
                                gStyle->SetOptStat(kFALSE);

                                usleep( 100 );

                                cBeBoardFWMap[cBoardId]->fData->Initialise( EVENT_NUMBER, pGlib );

                                while(!(cBeBoardFWMap[cBoardId]->fStop))
                                {

                                    cCbcInterface.Start(cBeBoardVec[j]);
                                    cCbcInterface.ReadData(cBeBoardVec[j], cNthAcq, true );
                                    cCbcInterface.Stop(cBeBoardVec[j], cNthAcq );

                                    bool cFillDataStream( false );

                                    const Event *cEvent = cBeBoardFWMap[cBoardId]->fData->GetNextEvent();

                                    while( cEvent )
                                    {

                                        if( cNevents != 0 && cN == cNevents )
                                        {
                                            cBeBoardFWMap[cBoardId]->fStop = true;
                                            break;
                                        }

                                        cCanvas->Divide(uint32_t(cBeBoardVec[j]->getNFe()),1);

                                        for(uint8_t i=0; i<pcBeBoardVec[j]->getNFe(); i++)
                                        {
                                            cfile << "Event N°" << cN+1 << std::endl;
                                            cfile << "FE " << uint32_t(i) << " :" << std::endl;

                                            if(cHist != NULL)
                                                delete cHist;

                                            cHist = new TH1F("Histo_Hits", "Hit Counter", uint32_t(cBeBoardVec[j]->getModule(i)->getNCbc()), 0., uint32_t(cBeBoardVec[j]->getModule(i)->getNCbc()));

                                            cCanvas->cd(uint32_t(i));

                                            for(uint8_t j=0; j<cBeBoardVec[j]->getModule(i)->getNCbc(); j++)
                                            {
                                                uint32_t cNHits = 0;

                                                std::vector<bool> cDataBitVector = cEvent->DataBitVector(i,j);

                                                cfile << "CBC " << uint32_t(j) << " : " << cEvent->DataBitString(i,j) << std::endl;

                                                for(uint32_t i=0; i<cDataBitVector.size(); i++)
                                                {
                                                    if(cDataBitVector[i])
                                                    {
                                                        cNHits++;
                                                    }
                                                }

                                                cHist->Fill(uint32_t(j),cNHits);
                                                cHistMean->Fill(uint32_t(j),cNHits/cNevents);
                                            }

                                            cHist->Draw();
                                            cCanvas->Update();

                                            cfile << "\n";
                                        }

                                        cCanvas->Print(((boost::format("output/Histogram_Event_%d.pdf") %(cN)).str()).c_str());

                                        cEvent = cBeBoardFWMap[cBoardId]->fData->GetNextEvent();

                                        cFillDataStream = false;
                                        cN++;
                                    }

                                    if( cN == cNevents )
                                    {
                                        cBeBoardFWMap[cBoardId]->fStop = true;
                                        break;
                                    }

                                }

                                cHistMean->Draw();
                                cCanvasMean->Update();
                                cCanvas->Print("output/Histogram_Mean.pdf");

                                delete cHist;
                                delete cHistMean;
                                delete cCanvas;
                                delete cCanvasMean;

                                cfile.close();

                                std::cout << "*** Acquisition Run ***" << std::endl;
                            }
                            else if(j == cBeBoardVec.size())
                            {
                                std::cout << "ERROR : This board does not exist !" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                        }
                    break;


                    default :
                        std::cout << "*** This is not the option you are looking for... ***" << std::endl;
                        myflush( std::cin );
                        mypause();
                    break;

                }

            break;


            case 6:
                {

                    std::cout << "\n\n"<< std::endl;
                    std::cout << "****************************************************" << std::endl;
                    std::cout << "***             Configuration Recap'             ***" << std::endl;
                    std::cout << "****************************************************\n" << std::endl;

                    std::cout << "1: Full Recap'" << std::endl;
                    std::cout << "2: Fast Recap'\n" << std::endl;

                    std::cin >> i;

                    switch(i)
                    {

                        case 1:
                        {
                            std::cout << "\n\n"<< std::endl;
                            std::cout << "****************************************************" << std::endl;
                            std::cout << "***           Configuration Full Recap'          ***" << std::endl;
                            std::cout << "****************************************************\n" << std::endl;

                            for(BeBoardFWMap::iterator cIt = cBeBoardMap.begin(); cIt = cBeBoardMap.end(); ++cIt)
                            {

                                for(uint32_t m=0; m<cBeBoardVec.size(); m++)
                                {

                                    if(cBeBoardVec[m]->fBeId == cIt->first)
                                    {

                                        std::cout << "****************************************************" << std::endl;
                                        std::cout << "***               Board Configuration             ***" << std::endl;
                                        std::cout << "****************************************************\n" << std::endl;

                                        std::cout << " ---------------------- " << std::endl;
                                        std::cout << "| Number of Module : "<< uint32_t(cBeBoardVec[m]->getNFe()) << std::endl;
                                        std::cout << "| Be Id : " << uint32_t(cBeBoardVec[m]->getBeId()) << std::endl;
                                        std::cout << "| Shelve Id : " << uint32_t(cBeBoardVec[m]->getShelveId()) << std::endl;
                                        std::cout << " ---------------------- \n\n" << std::endl;

                                        std::cout << " *** Board Register Map *** " << std::endl;
                                        std::cout << " -------------------------------------------------------------- " << std::endl;

                                        BeBoardRegMap cGlibRegMap = cBeBoardVec[m]->getBeBoardRegMap();
                                        for(BeBoardRegMap::iterator cJt = cGlibRegMap.begin(); cJt != cGlibRegMap.end(); ++cJt )
                                        {
                                            std::cout << "| " << cJt->first << " : " << uint32_t(cJt->second) << std::endl;
                                        }

                                        std::cout << " -------------------------------------------------------------- \n\n" << std::endl;

                                        myflush( std::cin );
                                        mypause();

                                        std::cout << "****************************************************" << std::endl;
                                        std::cout << "***      Module and Cbc Configuration            ***" << std::endl;
                                        std::cout << "****************************************************" << std::endl;

                                        for(uint8_t k=0;k<cBeBoardVec[m]->getNFe();k++)
                                        {

                                            if(cBeBoardVec[m]->getModule(k+cMissedModule) == NULL)
                                            {
                                                k--;
                                                cMissedModule++;
                                            }

                                            else
                                            {

                                                std::cout << " \n\n*** Module *** " << std::endl;
                                                std::cout << " --------------------- " << std::endl;
                                                std::cout << "| Module Id : "<< uint32_t(cBeBoardVec[m]->getModule(k+cMissedModule)->fModuleId) << std::endl;
                                                std::cout << "| Number of Cbc : " << uint32_t(cBeBoardVec[m]->getModule(k+cMissedModule)->getNCbc()) << std::endl;
                                                std::cout << " --------------------- \n" << std::endl;

                                                cCbcInterface.ReadAllCbc(cBeBoardVec[m]->getModule(cModuleId));

                                                for(uint8_t j=0;j<cBeBoardVec[m]->getModule(k+cMissedModule)->getNCbc();j++)
                                                {
                                                    if(cBeBoardVec[m]->getModule(k+cMissedModule)->getCbc(j+cMissedCbc) == NULL)
                                                    {
                                                        j--;
                                                        cMissedCbc++;
                                                    }

                                                    else
                                                    {

                                                        std::cout << "     *** Cbc contained *** " << std::endl;
                                                        std::cout << "     ------------------------- " << std::endl;
                                                        std::cout << "    | Cbc Id : " << uint32_t(cBeBoardVec[m]->getModule(k+cMissedModule)->getCbc(j+cMissedCbc)->getCbcId()) << std::endl;
                                                        std::cout << "     ------------------------- " << std::endl;

                                                        CbcRegMap cCbcRegMap = cBeBoardVec[m]->getModule(k+cMissedModule)->getCbc(j+cMissedCbc)->getRegMap();

                                                        for(CbcRegMap::iterator cJt = cCbcRegMap.begin(); cJt != cCbcRegMap.end(); ++cJt)
                                                        {
                                                            std::cout << "    | " << cJt->first << " : 0x" << std::hex << uint32_t((cJt->second).fValue) << std::endl;
                                                        }
                                                        std::cout << "     ------------------------- " << std::endl;

                                                        mypause();
                                                    }
                                                }
                                            }

                                            cMissedCbc = 0;
                                        }

                                        cMissedModule = 0;
                                        break;
                                    }
                                }
                            }
                        }
                        break;

                        case 2:
                        {
                            std::cout << "\n\n\n\n"<< std::endl;
                            std::cout << "****************************************************" << std::endl;
                            std::cout << "***          Configuration Fast Recap'           ***" << std::endl;
                            std::cout << "****************************************************\n" << std::endl;

                            for(BeBoardFWMap::iterator cIt = cBeBoardMap.begin(); cIt = cBeBoardMap.end(); ++cIt)
                            {

                                for(uint32_t m=0; m<cBeBoardVec.size(); m++)
                                {

                                    if(cBeBoardVec[m]->fBeId == cIt->first)
                                    {

                                        std::cout << "****************************************************" << std::endl;
                                        std::cout << "***               Board Configuration            ***" << std::endl;
                                        std::cout << "****************************************************\n" << std::endl;

                                        std::cout << " ---------------------- " << std::endl;
                                        std::cout << "| Number of Module : "<< uint32_t(cBeBoardVec[m]->getNFe()) << " |" << std::endl;
                                        std::cout << "| Be Id : " << uint32_t(cBeBoardVec[m]->getBeId()) << "            |" << std::endl;
                                        std::cout << "| Shelve Id : " << uint32_t(cBeBoardVec[m]->getShelveId()) << "        |" << std::endl;
                                        std::cout << " ---------------------- \n\n" << std::endl;

                                        myflush( std::cin );
                                        mypause();

                                        std::cout << "****************************************************" << std::endl;
                                        std::cout << "***        Module and Cbc Configuration          ***" << std::endl;
                                        std::cout << "****************************************************" << std::endl;

                                        for(uint8_t k=0;k<cBeBoardVec[m]->getNFe();k++)
                                        {

                                            if(cBeBoardVec[m]->getModule(k+cMissedModule) == NULL)
                                            {
                                                k--;
                                                cMissedModule++;
                                            }

                                            else
                                            {

                                                std::cout << " \n\n*** Module *** " << std::endl;
                                                std::cout << " --------------------- " << std::endl;
                                                std::cout << "| Module Id : "<< uint32_t(cBeBoardVec[m]->getModule(k+cMissedModule)->fModuleId) << std::endl;
                                                std::cout << "| Number of Cbc : " << uint32_t(cBeBoardVec[m]->getModule(k+cMissedModule)->getNCbc()) << std::endl;
                                                std::cout << " --------------------- \n" << std::endl;

                                                cCbcInterface.UpdateAllCbc(cBeBoardVec[m]->getModule(cModuleId));

                                                for(uint8_t j=0;j<cBeBoardVec[m]->getModule(k+cMissedModule)->getNCbc();j++)
                                                {
                                                    if(cBeBoardVec[m]->getModule(k+cMissedModule)->getCbc(j+cMissedCbc) == NULL)
                                                    {
                                                        j--;
                                                        cMissedCbc++;
                                                    }

                                                    else
                                                    {
                                                        std::cout << "     *** Cbc contained *** " << std::endl;
                                                        std::cout << "     ------------------------- " << std::endl;
                                                        std::cout << "    | Cbc Id : " << uint32_t(cBeBoardVec[m]->getModule(k+cMissedModule)->getCbc(j+cMissedCbc)->getCbcId()) << std::endl;
                                                        std::cout << "     ------------------------- " << std::endl;
                                                    }
                                                }

                                                mypause();
                                            }

                                            cMissedCbc = 0;

                                        }

                                        cMissedModule = 0;
                                        break;
                                    }
                                }
                            }
                        }
                        break;

                        default:
                            std::cout << "*** This is not the option you are looking for... ***" << std::endl;
                            myflush( std::cin );
                            mypause();
                        break;

                }

            }

            break;


            case 0:
                std::cout << "Quit..." << std::endl;
            break;


            default:
                std::cout << "*** This is not the option you are looking for... ***" << std::endl;
                myflush( std::cin );
                mypause();
            break;

        }

    }
    while(i!=0);

    for(uint32_t m=0; m<cBeBoardVec.size(); m++)
    {

        for(uint8_t k=0;k<cBeBoardVec[m]->getNFe();k++)
        {

            if(cBeBoardVec[m]->getModule(k+cMissedModule) == NULL)
            {
                k--;
                cMissedModule++;
            }

            else
            {
                for(uint8_t j=0;j<cBeBoardVec[m]->getModule(k+cMissedModule)->getNCbc();j++)
                {
                    if(cBeBoardVec[m]->getModule(k+cMissedModule)->getCbc(j+cMissedCbc) == NULL)
                    {
                        j--;
                        cMissedCbc++;
                    }

                    else
                    {
                        cBeBoardVec[m]->getModule(k+cMissedModule)->getCbc(j+cMissedCbc)->writeRegValues((boost::format("output/output_%d_%d_%d.txt") %(uint32_t(cBeBoardVec[m]->fBeId)) %(uint32_t(k+cMissedModule)) %(uint32_t(j+cMissedCbc))).str());
                    }
                }

                cMissedCbc = 0;
            }
        }
    }

    std::cout << "\n\nEnd of the MCP program...\n\n" << std::endl;

    return 0;
}
