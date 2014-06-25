/*

  FileName : 					TestInterface.cpp
  Content : 					 TestInterface program, test the software
  Programmer : 				  Nicolas PIERRE
  Version : 					 1.a
  Date of creation : 	        24/06/14
  Support : 					 mail to : nicolas.pierre@cern.ch

*/

#include "../HWDescription/Description/Cbc.h"
#include "../HWDescription/Description/Module.h"
#include "../HWDescription/Description/Glib.h"
#include "../HWInterface/Interface/CbcInterface.h"
#include "../HWInterface/Interface/GLIBInterface.h"
#include "../HWDescription/Description/Definition.h"
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
    std::string cRegNode;
    uint32_t cValue;
    uint32_t cNthAcq;
    bool cBreakTrigger;
    int cMissedModule = 0;
    int cMissedCbc = 0;

    GlibInterface cGlibInterface("file:///afs/cern.ch/user/n/npierre/public/settings/connections.xml");
    CbcInterface cCbcInterface("file:///afs/cern.ch/user/n/npierre/public/settings/connections.xml");

    std::cout << "****************************************************************" << std::endl;
    std::cout << "****************************************************************" << std::endl;
    std::cout << "***            Test Program for Ph2 Base Software            ***" << std::endl;
    std::cout << "***                N. Pierre, L. Bidegain v1.a               ***" << std::endl;
    std::cout << "****************************************************************" << std::endl;
    std::cout << "****************************************************************" << std::endl;

    do
    {

        std::cout << "\n\n\n\n\n\n\n\n\nWhat do you want to do ?\n" << std::endl;
        std::cout << "1: Add a Module or a Cbc" << std::endl;
        std::cout << "2: Configure" << std::endl;
        std::cout << "3: Glib Manipulation" << std::endl;
        std::cout << "4: Cbc Manipulation" << std::endl;
        std::cout << "5: Acquisition-ish" << std::endl;
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
                    break;


                    case 2:
                        std::cout << "*** Add Cbc ***" << std::endl;
                        std::cout << "--> Which ModuleId ?" << std::endl;
                        std::cin >> cModuleId;
                        std::cout << "--> Which CbcId ?" << std::endl;
                        std::cin >> cCbcId;
                        cCbc.fCbcId=cCbcId;
                        if (cGlib.getModule(cModuleId) == NULL)
                        {
                            std::cout << "*** ERROR !!                                      ***" << std::endl;
                            std::cout << "*** This module does not exist !                  ***" << std::endl;
                            std::cout << "*** This is not the module you are looking for... ***" << std::endl;
                        }
                        else
                        {
                            cGlib.getModule(cModuleId)->addCbc(cCbc);
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
                std::cout << "3: Configure all Cbc" << std::endl;
                std::cout << "4: Configure all Cbc in all Module\n" << std::endl;

                std::cin >> i;

                switch(i)
                {

                    case 1:
                        std::cout << "*** Configure Glib ***" << std::endl;
                        cGlibInterface.ConfigureGlib(cGlib);
                    break;


                    case 2:
                        std::cout << "*** Configure Cbc ***" << std::endl;
                        std::cout << "--> Which ModuleId ?" << std::endl;
                        std::cin >> cModuleId;
                        std::cout << "--> Which CbcId ?" << std::endl;
                        std::cin >> cCbcId;
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
                        }
                    break;


                    case 3:
                        std::cout << "*** Configure all Cbc ***" << std::endl;
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
                                cCbcInterface.ConfigureCbc(cGlib.getModule(cModuleId)->getCbc(j));
                            }
                        }
                    break;


                    case 4:
                        std::cout << "*** Configure all Cbc in all Module ***" << std::endl;

                        for(uint8_t k=0;k<cGlib.getNFe();k++)
                        {
                            for(uint8_t j=0;j<cGlib.getModule(cModuleId)->getNCbc();j++)
                            {
                                cCbcInterface.ConfigureCbc(cGlib.getModule(cModuleId)->getCbc(j));
                            }
                        }
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
                std::cout << "****************************************************\n\n" << std::endl;

                std::cout << "1: Update both ways" << std::endl;
                std::cout << "2: Update one way\n\n" << std::endl;

                std::cin >> i;

                switch(i)
                {

                    case 1:
                        std::cout << "*** Update both ways ***" << std::endl;
                        std::cout << "--> Which Register ?" << std::endl;
                        std::cin.clear();
                        getline(std::cin,cRegNode);
                        std::cout << "--> Which Value ?" << std::endl;
                        std::cin >> cValue;
                        cGlibInterface.UpdateGlibWrite(cGlib,cRegNode,cValue);
                    break;


                    case 2:
                        std::cout << "*** Update one way ***" << std::endl;
                        std::cout << "--> Which Register ?" << std::endl;
                        std::cin.clear();
                        getline(std::cin,cRegNode);
                        std::cout << "--> Which Value ?" << std::endl;
                        std::cin >> cValue;
                        cGlibInterface.UpdateGlibRead(cGlib,cRegNode);
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
                std::cout << "****************************************************\n\n" << std::endl;

                std::cout << "1: Update both ways" << std::endl;
                std::cout << "2: Update one way" << std::endl;
                std::cout << "3: Write all Cbc" << std::endl;
                std::cout << "4: Read all Cbc\n\n" << std::endl;

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
                                std::cin.clear();
                                getline(std::cin,cRegNode);
                                std::cout << "--> Which Value ?" << std::endl;
                                std::cin >> cValue;
                                cCbcInterface.UpdateCbcWrite(cGlib.getModule(cModuleId)->getCbc(cCbcId),cRegNode,cValue);
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
                                std::cin.clear();
                                getline(std::cin,cRegNode);
                                cCbcInterface.UpdateCbcRead(cGlib.getModule(cModuleId)->getCbc(cCbcId),cRegNode);
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
                            std::cin.clear();
                            getline(std::cin,cRegNode);
                            std::cout << "--> Which Value ?" << std::endl;
                            std::cin >> cValue;
                            cCbcInterface.WriteBroadcast(cGlib.getModule(cModuleId),cRegNode,cValue);
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
                            std::cin.clear();
                            getline(std::cin,cRegNode);
                            cCbcInterface.ReadCbc(cGlib.getModule(cModuleId),cRegNode);
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
                std::cout << "****************************************************\n\n" << std::endl;

                std::cout << "1: Start" << std::endl;
                std::cout << "2: Pause" << std::endl;
                std::cout << "3: UnPause" << std::endl;
                std::cout << "4: Stop" << std::endl;
                std::cout << "5: Read Data\n\n" << std::endl;

                std::cin >> i;

                switch(i)
                {

                    case 1:
                        std::cout << "*** Start ***" << std::endl;
                        cGlibInterface.Start(cGlib);
                    break;


                    case 2:
                        std::cout << "*** Pause ***" << std::endl;
                        cGlibInterface.Pause(cGlib);
                    break;


                    case 3:
                        std::cout << "*** UnPause ***" << std::endl;
                        cGlibInterface.Unpause(cGlib);
                    break;


                    case 4:
                        std::cout << "*** Stop ***" << std::endl;
                        std::cout << "--> Nth Acq ?" << std::endl;
                        std::cin >> cNthAcq;
                        cGlibInterface.Stop(cGlib,cNthAcq);
                    break;


                    case 5:
                        std::cout << "*** Read Data ***" << std::endl;
                        std::cout << "--> Nth Acq ?" << std::endl;
                        std::cin >> cNthAcq;
                        std::cout << "--> Break trigger ?" << std::endl;
                        std::cin >> cBreakTrigger;
                        cGlibInterface.ReadData(cGlib,cNthAcq,cBreakTrigger);
                    break;


                    default :
                        std::cout << "*** This is not the option you are looking for... ***" << std::endl;
                    break;

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

    std::cout << "\n\n\n\nEnd of the program..." << std::endl;

    return 0;
}
