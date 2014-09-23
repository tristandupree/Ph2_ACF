/*

	FileName :                    SystemController.cc
	Content :                     Controller of the System, overall wrapper of the framework
	Programmer :                  Nicolas PIERRE
	Version :                     1.0
	Date of creation :            10/08/14
	Support :                     mail to : nicolas.pierre@cern.ch

*/

#include "SystemController.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

namespace Ph2_System
{

	SystemController::SystemController()
	{

	}

	SystemController::~SystemController()
    	{
	}

	void SystemController::InitializeHw(const std::string pFilename)
	{

		pugi::xml_document doc;
		uint32_t cShelveId, cBeId, cModuleId, cCbcId;
		uint32_t cNShelve = 0;
		int i,j;

		pugi::xml_parse_result result = doc.load_file(pFilename.c_str());

		if(!result)
		{
			std::cout << "ERROR :\n Unable to open the file : " << pFilename << std::endl;
			std::cout << "Error description : " << result.description() << std::endl;
			return;
		}

		std::cout<<"\n\n\n";
   		 for( i=0;i<80;i++)
		{
			std::cout<<"*";
		}
		std::cout<<"\n";
		for(j=0;j<40;j++)
		{
		std::cout<<" ";
		}
    		std::cout<<BOLDRED<<"HW SUMMARY: "<<RESET<<std::endl;
		for( i=0;i<80;i++)
		{
			std::cout<<"*";
		}
		std::cout<<"\n";
		std::cout<<"\n";
		for(pugi::xml_node ns = doc.child("HwDescription").child("Shelve");ns;ns=ns.next_sibling())
		{
			cShelveId = ns.attribute("Id0").as_int();
			fShelveVec.push_back(new Shelve(cShelveId));
			
			std::cout<<BOLDCYAN<<ns.name()<<"  "<<ns.first_attribute().name()<<" :"<<ns.attribute("Id").value() <<RESET<<std:: endl;
			

			for(pugi::xml_node nb = ns.child("BeBoard");nb;nb=nb.next_sibling())
			{
				
			std::cout<<BOLDCYAN<<"|"<<"----"<<nb.name()<<"  "<<nb.first_attribute().name()<<" :"<<nb.attribute("Id").value() <<RESET<<std:: endl;
			
				cBeId = nb.attribute("Id").as_int();
				BeBoard cBeBoard(cShelveId,cBeId);

				for (pugi::xml_node nr = nb.child("Register"); nr!=nb.child("Module"); nr = nr.next_sibling())
				{
					// std::cout<<BOLDCYAN<<"|"<<"	"<<"|"<<"_____"<<nr.name()<<"  "<<nr.first_attribute().name()<<" :"<<nr.attribute("name").value() <<RESET<<std:: endl;
					cBeBoard.setReg(std::string(nr.attribute("name").value()),atoi(nr.first_child().value()));
				}

				fShelveVec[cNShelve]->addBoard(cBeBoard);

				BeBoardFWInterface* cBeBoardFWInterface;

				if(std::string(nb.attribute("boardType").value()).compare(std::string("Glib")))
				{
					cBeBoardFWInterface = new GlibFWInterface(doc.child("HwDescription").child("Connections").attribute("name").value(),cBeId);
					fBeBoardFWMap[cBeId] = cBeBoardFWInterface;
				}
				/*else
					cBeBoardFWInterface = new OtherFWInterface();*/

				for(pugi::xml_node nm = nb.child("Module");nm;nm=nm.next_sibling())
				{
					std::cout<<BOLDCYAN<<"|"<<"	"<<"|"<<"----"<<nm.name()<<"  "<<nm.first_attribute().name()<<" :"<<nm.attribute("ModuleId").value() <<RESET<<std:: endl;
					cModuleId = nm.attribute("ModuleId").as_int();
					Module cModule(cShelveId,cBeId,nm.attribute("FMCId").as_int(),nm.attribute("FeId").as_int(),cModuleId);
					fShelveVec[cNShelve]->getBoard(cBeId)->addModule(cModule);

					for(pugi::xml_node nc = nm.child("CBC");nc;nc=nc.next_sibling())
					{
						std::cout<<BOLDCYAN<<"|"<<"	"<<"|"<<"	"<<"|"<<"----"<<nc.name()<<"  "<<nc.first_attribute().name()<<" :"<<nc.attribute("Id").value() <<RESET<<std:: endl;
						Cbc cCbc(cShelveId,cBeId,nm.attribute("FMCId").as_int(),nm.attribute("FeId").as_int(),nc.attribute("Id").as_int(),nc.attribute("configfile").value());
						for(pugi::xml_node ngr = nc.child("Register");ngr;ngr=ngr.next_sibling())
						{
							cCbc.setReg(std::string(ngr.attribute("name").value()),atoi(ngr.first_child().value()));
						}
						for(pugi::xml_node ng = nm.child("Global_CBC_Register");ng!=nm.child("CBC") && ng!=NULL;ng=ng.next_sibling())
						{
													// std::cout<<BOLDCYAN<<"|"<<"	"<<"|"<<"	"<<"|"<<"_____"<<ng.name()<<"  "<<ng.first_attribute().name()<<" :"<<ng.attribute("name").value() <<RESET<<std:: endl;
							if(ng != NULL){
								std::string regname = std::string(ng.attribute("name").value());
								uint32_t regvalue = atoi(ng.first_child().value());
								cCbc.setReg(regname,regvalue);
								std::cout<<BOLDCYAN<<"|"<<"	"<<"|"<<"	"<<"|"<<"----"<<ng.name()<<"  "<<ng.first_attribute().name()<<" :"<<ng.attribute("name").value() <<RESET<<std:: endl;
							}
						}
						fShelveVec[cNShelve]->getBoard(cBeId)->getModule(cModuleId)->addCbc(cCbc);
					}
				}

			}

			cNShelve++;
		}

		fBeBoardInterface = new BeBoardInterface(fBeBoardFWMap);
		fCbcInterface = new CbcInterface(fBeBoardFWMap);
std::cout<<"\n";
std::cout<<"\n";
 for( i=0;i<80;i++)
	{
	std::cout<<"*";
	}
std::cout<<"\n";
	for(j=0;j<40;j++)
		{
		std::cout<<" ";
		}
    std::cout<<BOLDRED<<"END OF HW SUMMARY: "<<RESET<<std::endl;
	for( i=0;i<80;i++)
	{
	std::cout<<"*";
	}
std::cout<<"\n";
std::cout<<"\n";
	}

	void SystemController::InitializeSettings(const std::string pFilename)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(pFilename.c_str());

		if(!result)
		{
			std::cout << "ERROR :\n Unable to open the file : " << pFilename << std::endl;
			std::cout << "Error description : " << result.description() << std::endl;
			return;
		}
		for(pugi::xml_node nSettings = doc.child("Settings"); nSettings; nSettings=nSettings.next_sibling())
		{
			for(pugi::xml_node nSetting = nSettings.child("Setting"); nSetting; nSetting=nSetting.next_sibling())
			{	
				if ( std::string(nSetting.first_child().value()).find("0x") != std::string::npos ) 
					{
					  fSettingsMap[nSetting.attribute("name").value()] = strtoul(std::string (nSetting.first_child().value()).c_str(), 0, 16 );
					  std:: cout <<RED<< "Setting"<<RESET<<" --" <<BOLDCYAN<< nSetting.attribute("name").value() <<RESET<< ":"<<BOLDYELLOW << strtoul(std::string(nSetting.first_child().value()).c_str(),0,10)<<RESET<<std:: endl;
					}
				else 
					{
					  fSettingsMap[nSetting.attribute("name").value()] = strtoul(std::string( nSetting.first_child().value()).c_str(), 0, 10 );			
                                	  std:: cout <<RED<< "Setting"<<RESET<<" --" <<BOLDCYAN<< nSetting.attribute("name").value() <<RESET<< ":"<<BOLDYELLOW << strtoul(std::string(nSetting.first_child().value()).c_str(),0,10)<<RESET<<std:: endl;
					}
                          }
		}
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

}
