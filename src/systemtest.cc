#include "../System/SystemController.h"

using namespace Ph2_System;

int main()
{
	SystemController cSystemController;
	cSystemController.InitializeHw("/afs/cern.ch/user/n/npierre/dev/System/HWDescription.xml");
	std::cout << uint32_t(cSystemController.fShelveVec[0]->getBoard(0)->getModule(3)->getCbc(0)->getCbcId()) << std::endl;
	cSystemController.ConfigureHw();
	std::cout << "Hurray" << std::endl;
	return 0;
}
