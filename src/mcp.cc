/*

  FileName :                     mcp.cpp
  Content :                      MCP Test Interface program, test the software
  Programmer :                   Nicolas PIERRE, Lorenzo Bidegain
  Version :                      0.3
  Date of creation :             24/06/14
  Support :                      mail to : nico.pierre@cern.ch, lorenzo.bidegain@cern.ch

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

    Glib cGlib;
    Module cModule;
    Cbc cCbc;
    uint32_t cModuleId, cCbcId, cShelveId, cBeId, cFMCId, cFeId;
    std::string cFilePath;
    std::string cRegNode;
    std::string cValue;
    uint32_t cValueHex;
    uint32_t cNthAcq;
    bool cBreakTrigger;
    int cMissedModule = 0;
    int cMissedCbc = 0;

    GlibInterface cGlibInterface(UHAL_CONNECTION_FILE);
    CbcInterface cCbcInterface(UHAL_CONNECTION_FILE);

    cGlibInterface.ConfigureGlib(cGlib);

    std::cout << "****************************************************************" << std::endl;
    std::cout << "****************************************************************" << std::endl;
    std::cout << "***          MCP Test Program for Ph2 Base Software          ***" << std::endl;
    std::cout << "***                N. Pierre, L. Bidegain v0.3               ***" << std::endl;
    std::cout << "****************************************************************" << std::endl;
    std::cout << "****************************************************************" << std::endl;

    do
    {

        std::cout << "\n\n\n\nWhat do you want to do ?\n" << std::endl;
        std::cout << "1: Add/Remove a Module or a Cbc" << std::endl;
        std::cout << "2: Configure" << std::endl;
        std::cout << "3: Glib Manipulation" << std::endl;
        std::cout << "4: Cbc Manipulation" << std::endl;
        std::cout << "5: Acquisition-ish" << std::endl;
        std::cout << "6: Configuration Recap" << std::endl;
        std::cout << "7: Multi-Threading Test" << std::endl;
        std::cout << "0: Quit\n" << std::endl;

        std::cin >> i;

        switch(i)
        {

            case 1:

                std::cout << "\n\n\n\n"<< std::endl;
                std::cout << "****************************************************" << std::endl;
                std::cout << "***            Add a Module or a Cbc             ***" << std::endl;
                std::cout << "****************************************************\n" << std::endl;

                std::cout << "1: Add Module" << std::endl;
                std::cout << "2: Add Cbc" << std::endl;
                std::cout << "3: Remove Module" << std::endl;
                std::cout << "4: Remove Cbc\n" << std::endl;

                std::cin >> i;

                switch(i)
                {

                    case 1:
                        std::cout << "*** Add Module ***" << std::endl;
                        std::cout << "--> Which ModuleId ?" << std::endl;
                        std::cin >> cModuleId;
                        if(cGlib.getModule(cModuleId) == NULL)
                        {
                            cModule.fModuleId=cModuleId;
                            cGlib.addModule(cModule);
                            std::cout << "*** Module Added ***" << std::endl;
                        }
                        else
                        {
                            std::cout << "ERROR : This module already exists !" << std::endl;
                            myflush( std::cin );
                            mypause();
                        }
                    break;


                    case 2:
                        std::cout << "*** Add Cbc ***" << std::endl;
                        std::cout << "1: Add Default Cbc" << std::endl;
                        std::cout << "2: Add Personalised Cbc\n" << std::endl;

                        std::cin >> i;

                        switch(i)
                        {

                            case 1:
                                std::cout << "*** Add Default Cbc ***" << std::endl;
                                std::cout << "--> Which ModuleId ?" << std::endl;
                                std::cin >> cModuleId;
                                if (cGlib.getModule(cModuleId) == NULL)
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
                                    if(cGlib.getModule(cModuleId)->getCbc(cCbcId) == NULL)
                                    {
                                        cCbc.fCbcId=cCbcId;
                                        cGlib.getModule(cModuleId)->addCbc(cCbc);
                                        std::cout << "*** Cbc Added ***" << std::endl;
                                    }
                                    else
                                    {
                                        std::cout << "ERROR : This cbc already exists !" << std::endl;
                                        myflush( std::cin );
                                        mypause();
                                    }
                                }
                            break;


                            case 2:
                                std::cout << "*** Add Personalised Cbc ***" << std::endl;
                                std::cout << "--> Which ModuleId ?" << std::endl;
                                std::cin >> cModuleId;
                                if (cGlib.getModule(cModuleId) == NULL)
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

                                    if(cGlib.getModule(cModuleId)->getCbc(cCbcId) == NULL)
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
                                                    cGlib.getModule(cModuleId)->addCbc(cCbcPers);
                                                    std::cout << "*** Cbc Added ***" << std::endl;
                                                }
                                            break;


                                            case 2:
                                                {
                                                    Cbc cCbcPers(cShelveId,cBeId,cFMCId,cFeId,cCbcId,FE0CBC1);
                                                    cGlib.getModule(cModuleId)->addCbc(cCbcPers);
                                                    std::cout << "*** Cbc Added ***" << std::endl;
                                                }
                                            break;


                                            case 3:
                                                {
                                                    Cbc cCbcPers(cShelveId,cBeId,cFMCId,cFeId,cCbcId,FE0CBC1HOLE);
                                                    cGlib.getModule(cModuleId)->addCbc(cCbcPers);
                                                    std::cout << "*** Cbc Added ***" << std::endl;
                                                }
                                            break;


                                            case 4:
                                                {
                                                    std::cout << "--> Enter your File Path " << std::endl;
                                                    std::cout << "(Absolute path please) " << std::endl;
                                                    std::cin >> cFilePath;
                                                    Cbc cCbcPers(cShelveId,cBeId,cFMCId,cFeId,cCbcId,cFilePath);
                                                    cGlib.getModule(cModuleId)->addCbc(cCbcPers);
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
                            break;

                            default:
                                std::cout << "*** This is not the option you are looking for... ***" << std::endl;
                            break;

                        }
                    break;


                    case 3:
                        std::cout << "*** Remove Module ***" << std::endl;
                        std::cout << "--> Which ModuleId ?" << std::endl;
                        std::cin >> cModuleId;
                        if (cGlib.getModule(cModuleId) == NULL)
                        {
                            std::cout << "*** ERROR !!                                      ***" << std::endl;
                            std::cout << "*** This module does not exist !                  ***" << std::endl;
                            std::cout << "*** This is not the module you are looking for... ***" << std::endl;
                        }
                        else
                        {
                            if(cGlib.removeModule(cModuleId))
                                std::cout << "*** Module removed ! ***" << std::endl;
                            else
                                std::cout << "*** ERROR in Module removal !! ***" << std::endl;
                        }
                    break;


                    case 4:
                        std::cout << "*** Remove Cbc ***" << std::endl;
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
                            std::cout << "--> Which CbcId ?" << std::endl;
                            std::cin >> cCbcId;
                            if(cGlib.getModule(cModuleId)->getCbc(cCbcId) == NULL)
                            {
                                std::cout << "*** ERROR !!                                      ***" << std::endl;
                                std::cout << "*** This Cbc does not exist !                     ***" << std::endl;
                                std::cout << "*** This is not the Cbc you are looking for...    ***" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                            else
                            {
                                if(cGlib.getModule(cModuleId)->removeCbc(cCbcId))
                                    std::cout << "*** Module removed ! ***" << std::endl;
                                else
                                    std::cout << "*** ERROR in Cbc removal !! ***" << std::endl;
                            }
                            std::cout << "*** Cbc Configured ***" << std::endl;
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

                std::cout << "1: Configure Glib" << std::endl;
                std::cout << "2: Configure Cbc" << std::endl;
                std::cout << "3: Configure all Cbcs in one Module" << std::endl;
                std::cout << "4: Configure all Cbcs in all Modules\n" << std::endl;

                std::cin >> i;

                switch(i)
                {

                    case 1:
                        std::cout << "*** Configure Glib ***" << std::endl;
                        cGlibInterface.ConfigureGlib(cGlib);
                        std::cout << "*** Glib Configured ***" << std::endl;
                    break;


                    case 2:
                        std::cout << "*** Configure Cbc ***" << std::endl;
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
                            std::cout << "--> Which CbcId ?" << std::endl;
                            std::cin >> cCbcId;
                            if(cGlib.getModule(cModuleId)->getCbc(cCbcId) == NULL)
                            {
                                std::cout << "*** ERROR !!                                      ***" << std::endl;
                                std::cout << "*** This Cbc does not exist !                     ***" << std::endl;
                                std::cout << "*** This is not the Cbc you are looking for...    ***" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                            else
                            {
                                cCbcInterface.ConfigureCbc(cGlib.getModule(cModuleId)->getCbc(cCbcId));
                            }
                            std::cout << "*** Cbc Configured ***" << std::endl;
                        }
                    break;


                    case 3:
                        std::cout << "*** Configure all Cbcs in one Module***" << std::endl;
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
                            for(uint8_t j=0;j<cGlib.getModule(cModuleId)->getNCbc();j++)
                            {
                                if(cGlib.getModule(cModuleId)->getCbc(j+cMissedCbc) == NULL)
                                {
                                    j--;
                                    cMissedCbc++;
                                }

                                else
                                {
                                    cCbcInterface.ConfigureCbc(cGlib.getModule(cModuleId)->getCbc(j+cMissedCbc));
                                }
                            }
                            cMissedCbc = 0;
                            std::cout << "*** All Cbcs in the Module Configured ***" << std::endl;
                        }
                    break;


                    case 4:
                        std::cout << "*** Configure all Cbcs in all Modules ***" << std::endl;

                        for(uint8_t k=0;k<cGlib.getNFe();k++)
                        {
                            if(cGlib.getModule(k+cMissedModule) == NULL)
                            {
                                k--;
                                cMissedModule++;
                            }

                            else
                            {
                                for(uint8_t j=0;j<cGlib.getModule(k+cMissedModule)->getNCbc();j++)
                                {
                                    if(cGlib.getModule(k+cMissedModule)->getCbc(j+cMissedCbc) == NULL)
                                    {
                                        j--;
                                        cMissedCbc++;
                                    }

                                    else
                                    {
                                        cCbcInterface.ConfigureCbc(cGlib.getModule(k+cMissedModule)->getCbc(j+cMissedCbc));
                                    }
                                }
                                cMissedCbc = 0;
                            }
                        }
                        cMissedModule = 0;
                        std::cout << "*** All Cbcs in all Modules Configured ***" << std::endl;
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
                std::cout << "***              Glib Manipulation               ***" << std::endl;
                std::cout << "****************************************************\n" << std::endl;

                std::cout << "1: Update both ways" << std::endl;
                std::cout << "2: Update one way" << std::endl;
                std::cout << "3: Get infos\n" << std::endl;

                std::cin >> i;

                switch(i)
                {

                    case 1:
                        std::cout << "*** Update both ways ***" << std::endl;
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
                            cGlibInterface.UpdateGlibWrite(cGlib,cRegNode,cValueHex);
                            std::cout << "*** Updated ***" << std::endl;
                        }
                    break;


                    case 2:
                        std::cout << "*** Update one way ***" << std::endl;
                        std::cout << "--> Which Register ?" << std::endl;
                        std::cin >> cRegNode;
                        cGlibInterface.UpdateGlibRead(cGlib,cRegNode);
                        std::cout << "*** Updated ***" << std::endl;
                    break;


                    case 3:
                        std::cout << "*** Get Infos ***" << std::endl;
                        cGlibInterface.getBoardInfo(cGlib);
                        std::cout << "*** Infos Get ***" << std::endl;
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

                std::cout << "1: Update both ways" << std::endl;
                std::cout << "2: Update one way" << std::endl;
                std::cout << "3: Write all Cbc" << std::endl;
                std::cout << "4: Read all Cbc" << std::endl;
                std::cout << "5: Hard Reset a Cbc" << std::endl;
                std::cout << "6: Fast Reset Cbc\n" << std::endl;

                std::cin >> i;

                switch(i)
                {

                    case 1:
                        std::cout << "*** Update both ways ***" << std::endl;
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
                            std::cout << "--> Which CbcId ?" << std::endl;
                            std::cin >> cCbcId;
                            if(cGlib.getModule(cModuleId)->getCbc(cCbcId) == NULL)
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
                                    cCbcInterface.UpdateCbcWrite(cGlib.getModule(cModuleId)->getCbc(cCbcId),cRegNode,cValueHex);
                                    std::cout << "*** Updated ***" << std::endl;
                                }
                            }
                        }
                    break;


                    case 2:
                        std::cout << "*** Update one way ***" << std::endl;
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
                            std::cout << "--> Which CbcId ?" << std::endl;
                            std::cin >> cCbcId;
                            if(cGlib.getModule(cModuleId)->getCbc(cCbcId) == NULL)
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
                                cCbcInterface.UpdateCbcRead(cGlib.getModule(cModuleId)->getCbc(cCbcId),cRegNode);
                                std::cout << "*** Updated ***" << std::endl;
                            }
                        }
                    break;


                    case 3:
                        std::cout << "*** Write all Cbc ***" << std::endl;
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
                                cCbcInterface.WriteBroadcast(cGlib.getModule(cModuleId),cRegNode,cValueHex);
                                std::cout << "*** All Cbc Written ***" << std::endl;
                            }
                        }
                    break;


                    case 4:
                        std::cout << "*** Read all Cbc ***" << std::endl;
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
                            std::cout << "--> Which Register ?" << std::endl;
                            std::cin >> cRegNode;
                            cCbcInterface.ReadCbc(cGlib.getModule(cModuleId),cRegNode);
                            std::cout << "*** All Cbc Read ***" << std::endl;
                        }
                    break;


                    case 5:
                        std::cout << "*** Cbc Hard Reset ***" << std::endl;
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
                            std::cout << "--> Which CbcId ?" << std::endl;
                            std::cin >> cCbcId;
                            if(cGlib.getModule(cModuleId)->getCbc(cCbcId) == NULL)
                            {
                                std::cout << "*** ERROR !!                                      ***" << std::endl;
                                std::cout << "*** This Cbc does not exist !                     ***" << std::endl;
                                std::cout << "*** This is not the Cbc you are looking for...    ***" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                            else
                            {
                                cCbcInterface.CbcHardReset(cGlib.getModule(cModuleId)->getCbc(cCbcId));
                            }
                        std::cout << "*** Cbc Hard Reset ***" << std::endl;
                        }
                    break;


                    case 6:
                        std::cout << "*** Cbc Fast Reset ***" << std::endl;
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
                            std::cout << "--> Which CbcId ?" << std::endl;
                            std::cin >> cCbcId;
                            if(cGlib.getModule(cModuleId)->getCbc(cCbcId) == NULL)
                            {
                                std::cout << "*** ERROR !!                                      ***" << std::endl;
                                std::cout << "*** This Cbc does not exist !                     ***" << std::endl;
                                std::cout << "*** This is not the Cbc you are looking for...    ***" << std::endl;
                                myflush( std::cin );
                                mypause();
                            }
                            else
                            {
                                cCbcInterface.CbcFastReset(cGlib.getModule(cModuleId)->getCbc(cCbcId));
                            }
                        std::cout << "*** Cbc Fast Reset ***" << std::endl;
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

                std::cout << "1: Start" << std::endl;
                std::cout << "2: Pause" << std::endl;
                std::cout << "3: UnPause" << std::endl;
                std::cout << "4: Stop" << std::endl;
                std::cout << "5: Read Data\n" << std::endl;

                std::cin >> i;

                switch(i)
                {

                    case 1:
                        std::cout << "*** Start ***" << std::endl;
                        cGlibInterface.Start(cGlib);
                        std::cout << "*** Started ***" << std::endl;
                    break;


                    case 2:
                        std::cout << "*** Pause ***" << std::endl;
                        cGlibInterface.Pause(cGlib);
                        std::cout << "*** Paused ***" << std::endl;
                    break;


                    case 3:
                        std::cout << "*** UnPause ***" << std::endl;
                        cGlibInterface.Unpause(cGlib);
                        std::cout << "*** UnPaused ***" << std::endl;
                    break;


                    case 4:
                        std::cout << "*** Stop ***" << std::endl;
                        std::cout << "--> Nth Acq ?" << std::endl;
                        std::cin >> cNthAcq;
                        cGlibInterface.Stop(cGlib,cNthAcq);
                        std::cout << "*** Stopped ***" << std::endl;
                    break;


                    case 5:
                        std::cout << "*** Run Acquisition ***" << std::endl;
                        cGlibInterface.Run(cGlib);
                        std::cout << "*** Acquisition Run ***" << std::endl;
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

                            std::cout << "****************************************************" << std::endl;
                            std::cout << "***               Glib Configuration             ***" << std::endl;
                            std::cout << "****************************************************\n" << std::endl;

                            std::cout << " ---------------------- " << std::endl;
                            std::cout << "| Number of Module : "<< uint32_t(cGlib.getNFe()) << std::endl;
                            std::cout << "| Be Id : " << uint32_t(cGlib.getBeId()) << std::endl;
                            std::cout << "| Shelve Id : " << uint32_t(cGlib.getShelveId()) << std::endl;
                            std::cout << " ---------------------- \n\n" << std::endl;

                            std::cout << " *** Glib Register Map *** " << std::endl;
                            std::cout << " -------------------------------------------------------------- " << std::endl;

                            GlibRegMap cGlibRegMap = cGlib.getGlibRegMap();
                            for(GlibRegMap::iterator cIt = cGlibRegMap.begin(); cIt != cGlibRegMap.end(); ++cIt )
                            {
                                std::cout << "| " << cIt->first << " : " << uint32_t(cIt->second) << std::endl;
                            }

                            std::cout << " -------------------------------------------------------------- \n\n" << std::endl;

                            myflush( std::cin );
                            mypause();

                            std::cout << "****************************************************" << std::endl;
                            std::cout << "***      Module and Cbc Configuration            ***" << std::endl;
                            std::cout << "****************************************************" << std::endl;

                            for(uint8_t k=0;k<cGlib.getNFe();k++)
                            {

                                if(cGlib.getModule(k+cMissedModule) == NULL)
                                {
                                    k--;
                                    cMissedModule++;
                                }

                                else
                                {

                                    std::cout << " \n\n*** Module *** " << std::endl;
                                    std::cout << " --------------------- " << std::endl;
                                    std::cout << "| Module Id : "<< uint32_t(cGlib.getModule(k+cMissedModule)->fModuleId) << std::endl;
                                    std::cout << "| Number of Cbc : " << uint32_t(cGlib.getModule(k+cMissedModule)->getNCbc()) << std::endl;
                                    std::cout << " --------------------- \n" << std::endl;

                                    for(uint8_t j=0;j<cGlib.getModule(k+cMissedModule)->getNCbc();j++)
                                    {
                                        if(cGlib.getModule(k+cMissedModule)->getCbc(j+cMissedCbc) == NULL)
                                        {
                                            j--;
                                            cMissedCbc++;
                                        }

                                        else
                                        {

                                            std::cout << "     *** Cbc contained *** " << std::endl;
                                            std::cout << "     ------------------------- " << std::endl;
                                            std::cout << "    | Cbc Id : " << uint32_t(cGlib.getModule(k+cMissedModule)->getCbc(j+cMissedCbc)->getCbcId()) << std::endl;
                                            std::cout << "     ------------------------- " << std::endl;

                                            CbcRegMap cCbcRegMap = cGlib.getModule(k+cMissedModule)->getCbc(j+cMissedCbc)->getRegMap();

                                            for(CbcRegMap::iterator cIt = cCbcRegMap.begin(); cIt != cCbcRegMap.end(); cIt++)
                                            {
                                                std::cout << "    | " << cIt->first << " : 0x" << std::hex << uint32_t((cIt->second).fValue) << std::endl;
                                            }
                                            std::cout << "     ------------------------- " << std::endl;

                                            mypause();
                                        }
                                    }
                                }

                                cMissedCbc = 0;
                            }

                            cMissedModule = 0;
                        }
                        break;

                        case 2:
                        {
                            std::cout << "\n\n\n\n"<< std::endl;
                            std::cout << "****************************************************" << std::endl;
                            std::cout << "***          Configuration Fast Recap'           ***" << std::endl;
                            std::cout << "****************************************************\n" << std::endl;

                            std::cout << "****************************************************" << std::endl;
                            std::cout << "***               Glib Configuration             ***" << std::endl;
                            std::cout << "****************************************************\n" << std::endl;

                            std::cout << " ---------------------- " << std::endl;
                            std::cout << "| Number of Module : "<< uint32_t(cGlib.getNFe()) << " |" << std::endl;
                            std::cout << "| Be Id : " << uint32_t(cGlib.getBeId()) << "            |" << std::endl;
                            std::cout << "| Shelve Id : " << uint32_t(cGlib.getShelveId()) << "        |" << std::endl;
                            std::cout << " ---------------------- \n\n" << std::endl;

                            myflush( std::cin );
                            mypause();

                            std::cout << "****************************************************" << std::endl;
                            std::cout << "***      Module and Cbc Configuration            ***" << std::endl;
                            std::cout << "****************************************************" << std::endl;

                            for(uint8_t k=0;k<cGlib.getNFe();k++)
                            {

                                if(cGlib.getModule(k+cMissedModule) == NULL)
                                {
                                    k--;
                                    cMissedModule++;
                                }

                                else
                                {

                                    std::cout << " \n\n*** Module *** " << std::endl;
                                    std::cout << " --------------------- " << std::endl;
                                    std::cout << "| Module Id : "<< uint32_t(cGlib.getModule(k+cMissedModule)->fModuleId) << std::endl;
                                    std::cout << "| Number of Cbc : " << uint32_t(cGlib.getModule(k+cMissedModule)->getNCbc()) << std::endl;
                                    std::cout << " --------------------- \n" << std::endl;

                                    for(uint8_t j=0;j<cGlib.getModule(k+cMissedModule)->getNCbc();j++)
                                    {
                                        if(cGlib.getModule(k+cMissedModule)->getCbc(j+cMissedCbc) == NULL)
                                        {
                                            j--;
                                            cMissedCbc++;
                                        }

                                        else
                                        {

                                            std::cout << "     *** Cbc contained *** " << std::endl;
                                            std::cout << "     ------------------------- " << std::endl;
                                            std::cout << "    | Cbc Id : " << uint32_t(cGlib.getModule(k+cMissedModule)->getCbc(j+cMissedCbc)->getCbcId()) << std::endl;
                                            std::cout << "    | VCth : 0x" << std::hex << uint32_t(cGlib.getModule(k+cMissedModule)->getCbc(j+cMissedCbc)->getVcth()) << std::endl;
                                            std::cout << "    | Trigger Latency : 0x" << std::hex << uint32_t(cGlib.getModule(k+cMissedModule)->getCbc(j+cMissedCbc)->getTriggerLatency()) << std::endl;
                                            std::cout << "     ------------------------- " << std::endl;
                                        }
                                    }

                                    mypause();
                                }

                                cMissedCbc = 0;

                            }

                            cMissedModule = 0;
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


            case 7:
                std::cout << "*** Writing stack... ***" << std::endl;
                cGlibInterface.StackReg(EXT_TRG,0);
                cGlibInterface.StackReg(FAKE_DATA,0);
                cGlibInterface.StackReg(CBC_STUB_LATENCY_FE1,0);
                cGlibInterface.StackReg(CBC_STUB_LATENCY_FE2,0);
                cGlibInterface.StackReg(CBC_TRIGGER_1SHOT,0);
                cGlibInterface.StackReg(CBC_STUB_LATENCY_FE2,1);
                std::cout << "*** Stack written !!! ***" << std::endl;
            break;


            case 8:
                std::cout << "*** Writing stack... ***" << std::endl;
                cGlibInterface.StackReg(BREAK_TRIGGER,0);
                cGlibInterface.StackReg(POLARITY,0);
                cGlibInterface.StackReg(NEG_LOGIC_CBC,0);
                cGlibInterface.StackReg(NEG_LOGIC_STTS,0);
                cGlibInterface.StackReg(CBC_TRIGGER_1SHOT,0);
                cGlibInterface.StackReg(CBC_STUB_LATENCY_FE2,1);
                std::cout << "*** Stack written !!! ***" << std::endl;
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

    for(uint8_t k=0;k<cGlib.getNFe();k++)
    {

        if(cGlib.getModule(k+cMissedModule) == NULL)
        {
            k--;
            cMissedModule++;
        }

        else
        {
            for(uint8_t j=0;j<cGlib.getModule(k+cMissedModule)->getNCbc();j++)
            {
                if(cGlib.getModule(k+cMissedModule)->getCbc(j+cMissedCbc) == NULL)
    			{
    				j--;
    				cMissedCbc++;
    			}

                else
                {
                    cGlib.getModule(k+cMissedModule)->getCbc(j+cMissedCbc)->writeRegValues((boost::format("output/output_%d_%d.txt") %(uint32_t(k+cMissedModule)) %(uint32_t(j+cMissedCbc))).str());
                }
            }

            cMissedCbc = 0;
        }
    }

    std::cout << "\n\nEnd of the MCP program...\n\n" << std::endl;

    return 0;
}
