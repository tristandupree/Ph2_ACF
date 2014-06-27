/*

  FileName : 					TestInterface.cpp
  Content : 					 TestInterface program, test the software
  Programmer : 				  Nicolas PIERRE, Lorenzo Bidegain
  Version : 					 1.a
  Date of creation : 	        24/06/14
  Support : 					 mail to : nicolas.pierre@cern.ch, lorenzo.bidegain@cern.ch

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
    uint32_t cModuleId, cCbcId;
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

    std::cout << "****************************************************************" << std::endl;
    std::cout << "****************************************************************" << std::endl;
    std::cout << "***            Test Program for Ph2 Base Software            ***" << std::endl;
    std::cout << "***                N. Pierre, L. Bidegain v1.a               ***" << std::endl;
    std::cout << "****************************************************************" << std::endl;
    std::cout << "****************************************************************" << std::endl;

    do
    {

        std::cout << "\n\n\n\nWhat do you want to do ?\n" << std::endl;
        std::cout << "1: Add a Module or a Cbc" << std::endl;
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

                std::cout << "1: Add Module" << std::endl;
                std::cout << "2: Add Cbc\n" << std::endl;

                std::cin >> i;

                switch(i)
                {

                    case 1:
                        std::cout << "*** Add Module ***" << std::endl;
                        std::cout << "--> Which ModuleId ?" << std::endl;
                        std::cin >> cModuleId;
                        cModule.fModuleId=cModuleId;
                        cGlib.addModule(cModule);
                        std::cout << "*** Module Added ***" << std::endl;
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
                                }
                                else
                                {
                                    std::cout << "--> Which CbcId ?" << std::endl;
                                    std::cin >> cCbcId;
                                    cCbc.fCbcId=cCbcId;
                                    cGlib.getModule(cModuleId)->addCbc(cCbc);
                                }
                                std::cout << "*** Cbc Added ***" << std::endl;
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
                                }
                                else
                                {
                                    std::cout << "--> Which CbcId ?" << std::endl;
                                    std::cin >> cCbcId;

                                    std::cout << "*** Choose your Config File ***" << std::endl;
                                    std::cout << "1: FE0CBC0HOLE" << std::endl;
                                    std::cout << "2: FE0CBC1" << std::endl;
                                    std::cout << "3: FE0CBC1HOLE" << std::endl;
                                    std::cout << "4: Your own Config File" << std::endl;
                                    std::cin >> i;

                                    switch(i)
                                    {

                                        case 1:
                                            {
                                                Cbc cCbcPers(0,0,0,0,cCbcId,FE0CBC0HOLE);
                                                cGlib.getModule(cModuleId)->addCbc(cCbcPers);
                                            }
                                        break;


                                        case 2:
                                            {
                                                Cbc cCbcPers(0,0,0,0,cCbcId,FE0CBC0HOLE);
                                                cGlib.getModule(cModuleId)->addCbc(cCbcPers);
                                            }
                                        break;


                                        case 3:
                                            {
                                                Cbc cCbcPers(0,0,0,0,cCbcId,FE0CBC0HOLE);
                                                cGlib.getModule(cModuleId)->addCbc(cCbcPers);
                                            }
                                        break;


                                        case 4:
                                            {
                                                std::cout << "--> Enter your File Path " << std::endl;
                                                std::cout << "(Absolute path please) " << std::endl;
                                                std::cin >> cFilePath;
                                                Cbc cCbcPers(0,0,0,0,cCbcId,cFilePath);
                                                cGlib.getModule(cModuleId)->addCbc(cCbcPers);
                                            }
                                        break;


                                        default:
                                            std::cout << "*** This is not the option you are looking for... ***" << std::endl;
                                        break;

                                    }

                                }
                                std::cout << "*** Cbc Added ***" << std::endl;
                            break;

                            default:
                                std::cout << "*** This is not the option you are looking for... ***" << std::endl;
                            break;

                        }
                    break;


                    default:
                        std::cout << "*** This is not the option you are looking for... ***" << std::endl;
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
                    break;

                }

            break;


            case 3:

                std::cout << "\n\n\n\n"<< std::endl;
                std::cout << "****************************************************" << std::endl;
                std::cout << "***              Glib Manipulation               ***" << std::endl;
                std::cout << "****************************************************\n" << std::endl;

                std::cout << "1: Update both ways" << std::endl;
                std::cout << "2: Update one way\n" << std::endl;

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


                    default:
                        std::cout << "*** This is not the option you are looking for... ***" << std::endl;
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
                        std::cout << "*** Read Data ***" << std::endl;
                        std::cout << "--> Nth Acq ?" << std::endl;
                        std::cin >> cNthAcq;
                        std::cout << "--> Break trigger ?" << std::endl;
                        std::cin >> cBreakTrigger;
                        cGlibInterface.ReadData(cGlib,cNthAcq,cBreakTrigger);
                        std::cout << "*** Data Read ***" << std::endl;
                    break;


                    default :
                        std::cout << "*** This is not the option you are looking for... ***" << std::endl;
                    break;

                }

            break;


            case 6:
                {

                    std::cout << "\n\n\n\n"<< std::endl;
                    std::cout << "****************************************************" << std::endl;
                    std::cout << "***              Configuration Recap'            ***" << std::endl;
                    std::cout << "****************************************************\n" << std::endl;

                    std::cout << "****************************************************" << std::endl;
                    std::cout << "***               Glib Configuration             ***" << std::endl;
                    std::cout << "****************************************************\n" << std::endl;

                    std::cout << " ---------------------- " << std::endl;
                    std::cout << "| Number of Module : "<< uint32_t(cGlib.getNFe()) << " |" << std::endl;
                    std::cout << "| Be Id : " << uint32_t(cGlib.getBeId()) << "            |" << std::endl;
                    std::cout << "| Shelve Id : " << uint32_t(cGlib.getShelveId()) << "        |" << std::endl;
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
                            std::cout << " ------------------- " << std::endl;
                            std::cout << "| Module Id : "<< uint32_t(cGlib.getModule(k+cMissedModule)->fModuleId) << "     |" << std::endl;
                            std::cout << "| Number of Cbc : " << uint32_t(cGlib.getModule(k+cMissedModule)->getNCbc()) << " |" << std::endl;
                            std::cout << " ------------------- \n" << std::endl;

                            for(uint8_t j=0;j<cGlib.getModule(k+cMissedModule)->getNCbc();j++)
                            {
                                if(cGlib.getModule(k+cMissedModule)->getCbc(j+cMissedCbc) == NULL)
                                {
                                    j--;
                                    cMissedCbc++;
                                }

                                else
                                {
                                    cGlib.getModule(k+cMissedModule)->getCbc(j+cMissedCbc)->writeRegValues((boost::format("/afs/cern.ch/user/n/npierre/public/settings/output_%d_%d.txt") %(uint32_t(k+cMissedModule)) %(uint32_t(j+cMissedCbc))).str());

                                    std::cout << "     *** Cbc contained *** " << std::endl;
                                    std::cout << "     ----------------------- " << std::endl;
                                    std::cout << "    | Cbc Id : "<< uint32_t(cGlib.getModule(k+cMissedModule)->getCbc(j+cMissedCbc)->getCbcId()) << "            |" << std::endl;
                                    std::cout << "    | VCth : " << uint32_t(cGlib.getModule(k+cMissedModule)->getCbc(j+cMissedCbc)->getVcth()) << "            |" << std::endl;
                                    std::cout << "    | Trigger Latency : " << uint32_t(cGlib.getModule(k+cMissedModule)->getCbc(j+cMissedCbc)->getTriggerLatency()) << " |" << std::endl;
                                    std::cout << "     ----------------------- " << std::endl;
                                }
                            }

                            cMissedCbc = 0;

                            myflush( std::cin );
                            mypause();
                        }

                    }

                    cMissedModule = 0;

                }
            break;


            case 0:
                std::cout << "Quit..." << std::endl;
            break;


            default:
                std::cout << "*** This is not the option you are looking for... ***" << std::endl;
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
                    cGlib.getModule(k+cMissedModule)->getCbc(j+cMissedCbc)->writeRegValues((boost::format("/afs/cern.ch/user/n/npierre/public/settings/output_%d_%d.txt") %(uint32_t(k+cMissedModule)) %(uint32_t(j+cMissedCbc))).str());
                }
            }

            cMissedCbc = 0;
        }
    }

    std::cout << "\n\nEnd of the program...\n\n" << std::endl;

    return 0;
}
