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

	void SystemController::InitializeHw(std::string pFilename)
	{

		pugi::xml_document doc;
		uint32_t cShelveId, cBeId, cModuleId, cCbcId;
		uint32_t cNShelve = 0;

		if (!doc.load_file(pFilename.c_str()))
		{
			std::cout << "ERROR : Unable to open the file : " << pFilename << std::endl;
			return;
		}

		for(pugi::xml_node ns = doc.child("Shelf");ns;ns=ns.next_sibling())
		{
			cShelveId = ns.attribute("Id0").as_int();
			fShelveVec.push_back(new Shelve(cShelveId));

			for(pugi::xml_node nb = ns.child("BeBoard");nb;nb=nb.next_sibling())
			{

				cBeId = nb.attribute("Id").as_int();
				BeBoard cBeBoard(cShelveId,cBeId);

				for (pugi::xml_node nr = nb.child("Register"); nr!=nb.child("Module"); nr = nr.next_sibling())
				{
					cBeBoard.setReg(std::string(nr.attribute("name").value()),atoi(nr.first_child().value()));
				}

				fShelveVec[cNShelve]->addBoard(cBeBoard);

				BeBoardFWInterface* cBeBoardFWInterface;
				if(nb.attribute("boardType").value() == "GLIB")
				{
					cBeBoardFWInterface = new GlibFWInterface(UHAL_CONNECTION_FILE,cBeId);
					fBoardFWMap[cBeId] = cBeBoardFWInterface;
				}
				/*else
					cBeBoardFWInterface = new OtherFWInterface();*/

				for(pugi::xml_node nm = nb.child("Module");nm;nm=nm.next_sibling())
				{

					cModuleId = nm.attribute("ModuleId").as_int();
					Module cModule(cShelveId,cBeId,nm.attribute("FMCId").as_int(),nm.attribute("FeId").as_int(),cModuleId);
					fShelveVec[cNShelve]->getBoard(cBeId)->addModule(cModule);

					for(pugi::xml_node nc = nm.child("CBC");nc;nc=nc.next_sibling())
					{
						Cbc cCbc(cShelveId,cBeId,nm.attribute("FMCId").as_int(),nm.attribute("FeId").as_int(),nc.attribute("Id").as_int(),nc.attribute("configfile").value());
						for(pugi::xml_node ng = nm.child("Global_CBC_Register");ng!=nm.child("CBC");ng=ng.next_sibling())
						{
							cCbc.setReg(std::string(ng.attribute("name").value()),atoi(ng.first_child().value()));
						}
						for(pugi::xml_node ngr = nc.child("Register");ngr;ngr=ngr.next_sibling())
						{
							cCbc.setReg(std::string(ngr.attribute("name").value()),atoi(ngr.first_child().value()));
						}
						fShelveVec[cNShelve]->getBoard(cBeId)->getModule(cModuleId)->addCbc(cCbc);
					}
				}

			}

			cNShelve++;
		}

		fBeBoardInterface = new BeBoardInterface(fBoardFWMap);
		fCbcInterface = new CbcInterface(fBoardFWMap);

	}

	void SystemController::ConfigureHw()
	{
		uint32_t cMissedBoard, cMissedModule, cMissedCbc;

		for(uint32_t i=0; i<fShelveVec.size(); i++)
		{
			fBeBoardInterface.ConfigureBoard(fShelveVec[i]);

			cMissedBoard = 0;

			for(uint32_t j=0; j<fShelveVec[i]->getNBoard(); j++)
			{
				if(fShelveVec[i]->getBoard(j+cMissedBoard) == NULL)
				{
					j--;
					cMissedBoard++;
				}

				else
				{
					cMissedModule = 0;

					for(uint32_t k=0; k<fShelveVec[i]->getBoard(j+cMissedBoard)->getNFe(); k++)
					{
						if(fShelveVec[i]->getBoard(j+cMissedBoard)->getModule(k+cMissedModule) == NULL)
						{
							k--;
							cMissedModule++;
						}

						else
						{
							cMissedCbc = 0;

							for(uint32_t m=0; m<fShelveVec[i]->getBoard(j+cMissedBoard)->getModule(k+cMissedModule)->getNCbc(); m++)
							{
								if(fShelveVec[i]->getBoard(j+cMissedBoard)->getModule(k+cMissedModule)->getCbc(m+cMissedCbc) == NULL)
								{
									m--;
									cMissedCbc++;
								}

								else
								{
									fCbcInterface.ConfigureCbc(fShelveVec[i]->getBoard(j+cMissedBoard)->getModule(k+cMissedModule)->getCbc(m+cMissedCbc));
								}
							}
						}
					}
				}
			}
		}
	}

}
