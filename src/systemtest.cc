#include "../System/SystemController.h"

using namespace Ph2_System;

int main()
{
	SystemController cSystemController;
	cSystemController.InitializeHw(XML_DESCRIPTION_FILE);
	std::cout << uint32_t(cSystemController.fShelveVec[0]->getBoard(0)->getModule(0)->getCbc(0)->getCbcId()) << std::endl;
	cSystemController.ConfigureHw();
	std::cout << "Hurray" << std::endl;
	return 0;
}
