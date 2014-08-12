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
#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../HWInterface/Utilities.h"
#include <boost/format.hpp>
#include <TH1F.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TApplication.h>
#include <TROOT.h>

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

int main(int argc, char* argv[])
{
    int i;

<<<<<<< HEAD
    Glib cGlib;
    Module cModule;
    Cbc* cCbc;
    uint32_t cModuleId, cCbcId, cShelveId, cBeId, cFMCId, cFeId, cNbCbc;
=======
    BeBoard* cBeBoard;
    Module* cModule;
    Cbc* cCbc;
    uint32_t cBoardId, cModuleId, cCbcId, cShelveId, cBeId, cFMCId, cFeId, cNFe;
>>>>>>> origin/Dev
    std::string cFilePath;
    std::string cRegNode;
    std::string cValue;
    uint32_t cValueHex;
    uint32_t cNthAcq;
    bool cBreakTrigger;
    int cMissedModule = 0;
    int cMissedCbc = 0;

<<<<<<< HEAD
    GlibInterface* cGlibInterface = new GlibInterface(UHAL_CONNECTION_FILE,0);
    CbcInterface cCbcInterface(UHAL_CONNECTION_FILE);

=======
    TApplication cApp("Root Application", &argc, argv);

    BeBoardFWMap cBeBoardFWMap;
    std::vector<BeBoard*> cBeBoardVec;

    BeBoardFWInterface* cBeBoardFWInterface;
    BeBoardInterface cBeBoardInterface(cBeBoardFWMap);
    CbcInterface cCbcInterface(cBeBoardFWMap);

>>>>>>> origin/Dev
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
                            cBeBoardFWInterface = new BeBoardFWInterface(UHAL_CONNECTION_FILE,cBoardId);
                            if(cBeBoardFWInterface->getBoardType() == "GLIB")
                            {
                                delete cBeBoardFWInterface;
                                cBeBoardFWMap[cBoardId] = new GlibFWInterface(UHAL_CONNECTION_FILE,cBoardId);
                                cBeBoardVec.push_back(new BeBoard(cShelveId,cBoardId,cNFe));
                                std::cout << "*** Board Added ***" << std::endl;
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

                                    cModule = new Module(cBeBoardVec[j]->fShelveId,cBeBoardVec[j]->fBeId,cFMCId,cFeId,cModuleId);

                                    cBeBoardVec[j]->addModule(*cModule);
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
<<<<<<< HEAD
                        std::cout << "3: Add a 2Cbc Structure" << std::endl;
                        std::cout << "4: Add a 8Cbc Structure" << std::endl;
=======
                        std::cout << "3: Add a 2Cbc Hybrid Structure" << std::endl;
                        std::cout << "4: Add a 8Cbc Hybrid Structure\n" << std::endl;
>>>>>>> origin/Dev

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
<<<<<<< HEAD
                                        delete cGlibInterface;
                                        cGlibInterface = new GlibInterface(UHAL_CONNECTION_FILE,uint32_t(cGlib.getModule(cModuleId)->getNCbc()));
                                        cGlibInterface->ConfigureGlib(cGlib);

                                        cCbc = new Cbc(0,0,0,0,cCbcId,DEFAULT_FILE);
                                        cGlib.getModule(cModuleId)->addCbc(*cCbc);
                                        delete cCbc;
                                        std::cout << "*** Cbc Added ***" << std::endl;
=======
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
>>>>>>> origin/Dev
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

<<<<<<< HEAD
                                            case 1:
                                                {
                                                    delete cGlibInterface;
                                                    cGlibInterface = new GlibInterface(UHAL_CONNECTION_FILE,uint32_t(cGlib.getModule(cModuleId)->getNCbc()));
                                                    cGlibInterface->ConfigureGlib(cGlib);

                                                    Cbc cCbcPers(cShelveId,cBeId,cFMCId,cFeId,cCbcId,FE0CBC0HOLE);
                                                    cGlib.getModule(cModuleId)->addCbc(cCbcPers);
                                                    std::cout << "*** Cbc Added ***" << std::endl;
                                                }
                                            break;
=======
                                            if(cBeBoardVec[j]->getModule(cModuleId)->getCbc(cCbcId) == NULL)
                                            {
>>>>>>> origin/Dev

                                                std::cout << "--> Which ShelveId ?" << std::endl;
                                                std::cout << "(Put 0 if you want it per default)" << std::endl;
                                                std::cin >> cShelveId;

<<<<<<< HEAD
                                            case 2:
                                                {
                                                    delete cGlibInterface;
                                                    cGlibInterface = new GlibInterface(UHAL_CONNECTION_FILE,uint32_t(cGlib.getModule(cModuleId)->getNCbc()));
                                                    cGlibInterface->ConfigureGlib(cGlib);

                                                    Cbc cCbcPers(cShelveId,cBeId,cFMCId,cFeId,cCbcId,FE0CBC1);
                                                    cGlib.getModule(cModuleId)->addCbc(cCbcPers);
                                                    std::cout << "*** Cbc Added ***" << std::endl;
                                                }
                                            break;
=======
                                                std::cout << "--> Which BeId ?" << std::endl;
                                                std::cout << "(Put 0 if you want it per default)" << std::endl;
                                                std::cin >> cBeId;
>>>>>>> origin/Dev

                                                std::cout << "--> Which FMCId ?" << std::endl;
                                                std::cout << "(Put 0 if you want it per default)" << std::endl;
                                                std::cin >> cFMCId;

<<<<<<< HEAD
                                            case 3:
                                                {
                                                    delete cGlibInterface;
                                                    cGlibInterface = new GlibInterface(UHAL_CONNECTION_FILE,uint32_t(cGlib.getModule(cModuleId)->getNCbc()));
                                                    cGlibInterface->ConfigureGlib(cGlib);

                                                    Cbc cCbcPers(cShelveId,cBeId,cFMCId,cFeId,cCbcId,FE0CBC1HOLE);
                                                    cGlib.getModule(cModuleId)->addCbc(cCbcPers);
                                                    std::cout << "*** Cbc Added ***" << std::endl;
                                                }
                                            break;
=======
                                                std::cout << "--> Which FeId ?" << std::endl;
                                                std::cout << "(Put 0 if you want it per default)" << std::endl;
                                                std::cin >> cFeId;
>>>>>>> origin/Dev

                                                std::cout << "--> Choose your Config File" << std::endl;
                                                std::cout << "1: FE0CBC0HOLE" << std::endl;
                                                std::cout << "2: FE0CBC1" << std::endl;
                                                std::cout << "3: FE0CBC1HOLE" << std::endl;
                                                std::cout << "4: Your own Config File" << std::endl;
                                                std::cin >> i;

                                                switch(i)
                                                {
<<<<<<< HEAD
                                                    std::cout << "--> Enter your File Path " << std::endl;
                                                    std::cout << "(Absolute path please) " << std::endl;
                                                    std::cin >> cFilePath;

                                                    delete cGlibInterface;
                                                    cGlibInterface = new GlibInterface(UHAL_CONNECTION_FILE,uint32_t(cGlib.getModule(cModuleId)->getNCbc()));
                                                    cGlibInterface->ConfigureGlib(cGlib);

                                                    Cbc cCbcPers(cShelveId,cBeId,cFMCId,cFeId,cCbcId,cFilePath);
                                                    cGlib.getModule(cModuleId)->addCbc(cCbcPers);
                                                    std::cout << "*** Cbc Added ***" << std::endl;
                                                }
                                            break;
=======
>>>>>>> origin/Dev

                                                    case 1:
                                                        {
                                                            Cbc cCbcPers(cShelveId,cBeId,cFMCId,cFeId,cCbcId,FE0CBC0HOLE);
                                                            cBeBoardVec[j]->getModule(cModuleId)->addCbc(cCbcPers);
                                                            cCbcInterface.ConfigureCbc(cBeBoardVec[j]->getModule(cModuleId)->getCbc(cCbcId));
                                                            std::cout << "*** Cbc Added ***" << std::endl;
                                                        }
                                                    break;


                                                    case 2:
                                                        {
                                                            Cbc cCbcPers(cShelveId,cBeId,cFMCId,cFeId,cCbcId,FE0CBC1);
                                                            cBeBoardVec[j]->getModule(cModuleId)->addCbc(cCbcPers);
                                                            cCbcInterface.ConfigureCbc(cBeBoardVec[j]->getModule(cModuleId)->getCbc(cCbcId));
                                                            std::cout << "*** Cbc Added ***" << std::endl;
                                                        }
                                                    break;


                                                    case 3:
                                                        {
                                                            Cbc cCbcPers(cShelveId,cBeId,cFMCId,cFeId,cCbcId,FE0CBC1HOLE);
                                                            cBeBoardVec[j]->getModule(cModuleId)->addCbc(cCbcPers);
                                                            cCbcInterface.ConfigureCbc(cBeBoardVec[j]->getModule(cModuleId)->getCbc(cCbcId));
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
                                                            cCbcInterface.ConfigureCbc(cBeBoardVec[j]->getModule(cModuleId)->getCbc(cCbcId));
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
<<<<<<< HEAD
                                    delete cGlibInterface;
                                    cGlibInterface = new GlibInterface(UHAL_CONNECTION_FILE,2);
                                    cGlibInterface->ConfigureGlib(cGlib);

                                    for(uint8_t i=0; i<2; i++)
=======

                                    if(cBeBoardVec[j]->fBeId == cBoardId)
>>>>>>> origin/Dev
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
<<<<<<< HEAD
                                            cCbc = new Cbc(0,0,0,0,i,(boost::format("settings/FE0CBC%dHole.txt") %(uint32_t(i))).str());
                                            cGlib.getModule(cModuleId)->addCbc(*cCbc);
                                            cCbcInterface.ConfigureCbc(cGlib.getModule(cModuleId)->getCbc(i));
                                            delete cCbc;
                                        }
                                    }

                                    std::cout << "*** 2Cbc Structure Added ***" << std::endl;
=======
                                            for(uint8_t i=0; i<2; i++)
                                            {
                                                if(cBeBoardVec[j]->getModule(cModuleId)->getCbc(i) == NULL)
                                                {
                                                    cCbc = new Cbc(0,0,0,0,i,(boost::format("settings/FE0CBC%d.txt") %(uint32_t(i))).str());
                                                    cBeBoardVec[j]->getModule(cModuleId)->addCbc(*cCbc);
                                                    cCbcInterface.ConfigureCbc(cBeBoardVec[j]->getModule(cModuleId)->getCbc(i));
                                                    delete cCbc;
                                                }
                                            }

                                            std::cout << "*** 2Cbc Hybrid Structure Added ***" << std::endl;
                                        }
                                    }
                                    else if(j == cBeBoardVec.size())
                                    {
                                        std::cout << "ERROR : This board does not exist !" << std::endl;
                                        myflush( std::cin );
                                        mypause();
                                    }
>>>>>>> origin/Dev
                                }
                            break;



                            case 4:
                                std::cout << "*** Add a 8Cbc Hybrid Structure ***" << std::endl;
                                std::cout << "--> Which BoardId ?" << std::endl;
                                std::cin >> cBoardId;

                                for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                                {
<<<<<<< HEAD
                                    delete cGlibInterface;
                                    cGlibInterface = new GlibInterface(UHAL_CONNECTION_FILE,8);
                                    cGlibInterface->ConfigureGlib(cGlib);

                                    for(uint8_t i=0; i<8; i++)
=======

                                    if(cBeBoardVec[j]->fBeId == cBoardId)
>>>>>>> origin/Dev
                                    {
                                        std::cout << "--> Which ModuleId ?" << std::endl;
                                        std::cin >> cModuleId;
                                        if (cBeBoardVec[j]->getModule(cModuleId) == NULL)
                                        {
<<<<<<< HEAD
                                            cCbc = new Cbc(0,0,0,0,i,(boost::format("settings/FE0CBC%d.txt") %(uint32_t(i))).str());
                                            cGlib.getModule(cModuleId)->addCbc(*cCbc);
                                            cCbcInterface.ConfigureCbc(cGlib.getModule(cModuleId)->getCbc(i));
                                            delete cCbc;
                                        }
                                    }

                                    std::cout << "*** 8Cbc Hybrid Structure Added ***" << std::endl;
=======
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
                                                    cCbcInterface.ConfigureCbc(cBeBoardVec[j]->getModule(cModuleId)->getCbc(i));
                                                    delete cCbc;
                                                }
                                            }

                                            std::cout << "*** 8Cbc Hybrid Structure Added ***" << std::endl;
                                        }
                                    }
                                    else if(j == cBeBoardVec.size())
                                    {
                                        std::cout << "ERROR : This board does not exist !" << std::endl;
                                        myflush( std::cin );
                                        mypause();
                                    }
>>>>>>> origin/Dev
                                }
                            break;


                            default:
                                std::cout << "*** This is not the option you are looking for... ***" << std::endl;
                            break;

                        }
                    break;


                    case 4:
                    {
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
<<<<<<< HEAD
                        std::cout << "*** Configure Glib ***" << std::endl;
                        cGlibInterface->ConfigureGlib(cGlib);
                        std::cout << "*** Glib Configured ***" << std::endl;
=======
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
>>>>>>> origin/Dev
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
                                    for(uint8_t k=0;k<cBeBoardVec[j]->getModule(cModuleId)->getNCbc();k++)
                                    {
                                        if(cBeBoardVec[j]->getModule(cModuleId)->getCbc(k+cMissedCbc) == NULL)
                                        {
                                            k--;
                                            cMissedCbc++;
                                        }

                                        else
                                        {
                                            cCbcInterface.ConfigureCbc(cBeBoardVec[j]->getModule(cModuleId)->getCbc(k+cMissedCbc));
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
                                        for(uint8_t m=0;j<cBeBoardVec[j]->getModule(k+cMissedModule)->getNCbc();m++)
                                        {
                                            if(cBeBoardVec[j]->getModule(k+cMissedModule)->getCbc(m+cMissedCbc) == NULL)
                                            {
                                                m--;
                                                cMissedCbc++;
                                            }

                                            else
                                            {
                                                cCbcInterface.ConfigureCbc(cBeBoardVec[j]->getModule(k+cMissedModule)->getCbc(m+cMissedCbc));
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
<<<<<<< HEAD
                            cGlibInterface->UpdateGlibWrite(cGlib,cRegNode,cValueHex);
                            std::cout << "*** Updated ***" << std::endl;
=======

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
>>>>>>> origin/Dev
                        }
                    break;


                    case 2:
<<<<<<< HEAD
                        std::cout << "*** Update one way ***" << std::endl;
                        std::cout << "--> Which Register ?" << std::endl;
                        std::cin >> cRegNode;
                        cGlibInterface->UpdateGlibRead(cGlib,cRegNode);
                        std::cout << "*** Updated ***" << std::endl;
=======
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
>>>>>>> origin/Dev
                    break;


                    case 3:
                        std::cout << "*** Get Infos ***" << std::endl;
<<<<<<< HEAD
                        cGlibInterface->getBoardInfo(cGlib);
                        std::cout << "*** Infos Get ***" << std::endl;
=======
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
>>>>>>> origin/Dev
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
<<<<<<< HEAD
                        std::cout << "--> Which ModuleId ?" << std::endl;
                        std::cin >> cModuleId;
                        if(cGlib.getModule(cModuleId) == NULL)
                        {
                            std::cout << "*** ERROR !!                                      ***" << std::endl;
                            std::cout << "*** This module does not exist !                  ***" << std::endl;
                            std::cout << "*** This is not the module you are looking for... ***" << std::endl;
                            myflush( std::cin );
                            mypause();
                        }
                        else
=======
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        for(uint32_t j=0; j<cBeBoardVec.size(); j++)
>>>>>>> origin/Dev
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
<<<<<<< HEAD
                                    cCbcInterface.WriteCbc(cGlib.getModule(cModuleId)->getCbc(cCbcId),cRegNode,cValueHex);
                                    std::cout << "*** Cbc Written ***" << std::endl;
=======
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
>>>>>>> origin/Dev
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
<<<<<<< HEAD
                        std::cout << "--> Which ModuleId ?" << std::endl;
                        std::cin >> cModuleId;
                        if(cGlib.getModule(cModuleId) == NULL)
                        {
                            std::cout << "*** ERROR !!                                      ***" << std::endl;
                            std::cout << "*** This module does not exist !                  ***" << std::endl;
                            std::cout << "*** This is not the module you are looking for... ***" << std::endl;
                            myflush( std::cin );
                            mypause();
                        }
                        else
=======
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        for(uint32_t j=0; j<cBeBoardVec.size(); j++)
>>>>>>> origin/Dev
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
<<<<<<< HEAD
                                std::cout << "--> Which Register ?" << std::endl;
                                std::cin >> cRegNode;
                                cCbcInterface.UpdateCbc(cGlib.getModule(cModuleId)->getCbc(cCbcId),cRegNode);
                                std::cout << "*** Updated ***" << std::endl;
=======
                                std::cout << "ERROR : This board does not exist !" << std::endl;
                                myflush( std::cin );
                                mypause();
>>>>>>> origin/Dev
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
<<<<<<< HEAD
                        std::cout << "*** Update all Cbc ***" << std::endl;
                        std::cout << "--> Which ModuleId ?" << std::endl;
                        std::cin >> cModuleId;
                        if(cGlib.getModule(cModuleId) == NULL)
                        {
                            std::cout << "*** ERROR !!                                      ***" << std::endl;
                            std::cout << "*** This module does not exist !                  ***" << std::endl;
                            std::cout << "*** This is not the module you are looking for... ***" << std::endl;
                            myflush( std::cin );
                            mypause();
                        }
                        else
                        {
                            cCbcInterface.UpdateAllCbc(cGlib.getModule(cModuleId));
                            std::cout << "*** All Cbc Updated ***" << std::endl;
=======
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
>>>>>>> origin/Dev
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
<<<<<<< HEAD
                        std::cout << "*** Run Acquisition ***" << std::endl;

                        cGlibInterface->Run(cGlib);
=======
                    {
                        std::cout << "*** Run Acquisition ***" << std::endl;
                        std::cout << "--> Which BoardId ?" << std::endl;
                        std::cin >> cBoardId;

                        for(uint32_t j=0; j<cBeBoardVec.size(); j++)
                        {

                            if(cBeBoardVec[j]->fBeId == cBoardId)
                            {
                                std::ofstream cfile( "output/TestData.dat", std::ios::out | std::ios::trunc );

                                uint32_t cNthAcq = 0;
                                uint32_t cNevents = 500;
                                uint32_t cN = 0;

                                TCanvas *cCanvas = new TCanvas("Data Acq", "Different hits counters", 600, 400);
                                //cCanvas->Divide(uint32_t(cBeBoardVec[j]->getModule(0)->getNCbc()),1);
                                cCanvas->Divide(2,1);

                                std::vector<TH1F*> cHistVec;
                                gStyle->SetOptStat(kFALSE);

                                for(uint8_t m=0; m<cBeBoardVec[j]->getModule(0)->getNCbc(); m++)
                                {
                                    cHistVec.push_back(new TH1F(Form("Histo_Hits_CBC%d",m), Form("Occupancy_CBC%d",m), 255, -0.5, 254.5));
                                }

                                usleep( 100 );

                                cBeBoardFWMap[cBoardId]->fStop = false;

                                while(!(cBeBoardFWMap[cBoardId]->fStop))
                                {

                                    cBeBoardInterface.Start(cBeBoardVec[j]);
                                    cBeBoardInterface.ReadData(cBeBoardVec[j], cNthAcq, true );
                                    cBeBoardInterface.Stop(cBeBoardVec[j], cNthAcq );

                                    bool cFillDataStream( false );

                                    const Event *cEvent = cBeBoardFWMap[cBoardId]->fData->GetNextEvent();

                                    while( cEvent )
                                    {

                                        if( cNevents != 0 && cN == cNevents )
                                        {
                                            cBeBoardFWMap[cBoardId]->fStop = true;
                                            break;
                                        }

                                        for(uint8_t i=0; i<cBeBoardVec[j]->getNFe(); i++)
                                        {

                                            cfile << "Event N°" << cN+1 << std::endl;
                                            cfile << "FE " << uint32_t(i) << " :" << std::endl;

                                            for(uint8_t m=0; m<cBeBoardVec[j]->getModule(i)->getNCbc(); m++)
                                            {
                                                uint32_t cNHits = 0;

                                                std::vector<bool> cDataBitVector = cEvent->DataBitVector(i,m);

                                                cfile << "CBC " << uint32_t(j) << " : " << cEvent->DataBitString(i,m) << std::endl;

                                                for(uint32_t n=0; n<cDataBitVector.size(); n++)
                                                {
                                                    if(cDataBitVector[n])
                                                    {
                                                        cNHits++;
                                                        cHistVec[m]->Fill(n);
                                                    }
                                                }

                                            }

                                            cfile << "\n";
                                        }


                                        //cCanvas->Print(((boost::format("output/Histogram_Event_%d.pdf") %(cN)).str()).c_str());

                                        cEvent = cBeBoardFWMap[cBoardId]->fData->GetNextEvent();

                                        cFillDataStream = false;
                                        cN++;
                                    }

                                    for(uint8_t m=0; m<cHistVec.size(); m++)
                                    {
                                        cCanvas->cd(uint32_t(m));
                                        cHistVec[m]->Draw();
                                    }

                                    cCanvas->Update();

                                    if( cN == cNevents )
                                    {
                                        cBeBoardFWMap[cBoardId]->fStop = true;
                                        break;
                                    }

                                }
>>>>>>> origin/Dev

                                cCanvas->Print("output/Histogram_Mean.pdf");

                                //delete cCanvas;

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

                            for(BeBoardFWMap::iterator cIt = cBeBoardFWMap.begin(); cIt != cBeBoardFWMap.end(); ++cIt)
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

<<<<<<< HEAD
                                    cCbcInterface.UpdateAllCbc(cGlib.getModule(cModuleId));

                                    for(uint8_t j=0;j<cGlib.getModule(k+cMissedModule)->getNCbc();j++)
                                    {
                                        if(cGlib.getModule(k+cMissedModule)->getCbc(j+cMissedCbc) == NULL)
=======
                                        for(uint8_t k=0;k<cBeBoardVec[m]->getNFe();k++)
>>>>>>> origin/Dev
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

                            for(BeBoardFWMap::iterator cIt = cBeBoardFWMap.begin(); cIt != cBeBoardFWMap.end(); ++cIt)
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

<<<<<<< HEAD
                                    cCbcInterface.UpdateAllCbc(cGlib.getModule(cModuleId));

                                    for(uint8_t j=0;j<cGlib.getModule(k+cMissedModule)->getNCbc();j++)
                                    {
                                        if(cGlib.getModule(k+cMissedModule)->getCbc(j+cMissedCbc) == NULL)
=======
                                        for(uint8_t k=0;k<cBeBoardVec[m]->getNFe();k++)
>>>>>>> origin/Dev
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


<<<<<<< HEAD
            case 7:
                std::cout << "*** Writing stack... ***" << std::endl;
                cGlibInterface->StackReg(EXT_TRG,0);
                cGlibInterface->StackReg(FAKE_DATA,0);
                cGlibInterface->StackReg(CBC_STUB_LATENCY_FE1,0);
                cGlibInterface->StackReg(CBC_STUB_LATENCY_FE2,0);
                cGlibInterface->StackReg(CBC_TRIGGER_1SHOT,0);
                cGlibInterface->StackReg(CBC_STUB_LATENCY_FE2,1);
                std::cout << "*** Stack written !!! ***" << std::endl;
            break;


=======
>>>>>>> origin/Dev
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
                        cBeBoardVec[m]->getModule(k+cMissedModule)->getCbc(j+cMissedCbc)->saveRegMap((boost::format("output/output_%d_%d_%d.txt") %(uint32_t(cBeBoardVec[m]->fBeId)) %(uint32_t(k+cMissedModule)) %(uint32_t(j+cMissedCbc))).str());
                    }
                }

                cMissedCbc = 0;
            }
        }
    }

    //Root cApp
    cApp.Run();

    std::cout << "\n\nEnd of the MCP program...\n\n" << std::endl;

    return 0;
}
