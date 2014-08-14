#include "../System/SystemController.h"

using namespace Ph2_System;

int main()
{
	SystemController cSystemController;
	cSystemController.initializeHw("/afs/cern.ch/user/a/acagarwa/Ph2DAQ_dev/System/HWDescription.xml");
	std::cout << uint32_t(cSystemController.fShelveVec[0]->getBoard(2)->getModule(3)->getCbc(0)->getCbcId()) << 
std::endl;
	std::cout << "Hurray" << std::endl;
	return 0;
}
