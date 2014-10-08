/*

   FileName :                     test.cpp
   Content :                      Little test program to see if everything is working fine
   Programmer :                   Nicolas PIERRE, Lorenzo Bidegain
   Version :                      0.3
   Date of creation :             24/06/14
   Support :                      mail to : nico.pierre@cern.ch, lorenzo.bidegain@cern.ch

 */

#include "../HWDescription/Cbc.h"
#include "../HWDescription/Module.h"
#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../System/SystemController.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;
using namespace Ph2_System;

int main()
{

	SystemController cSystemController;

	cSystemController.InitializeHw( XML_DESCRIPTION_FILE_2CBC );
	cSystemController.ConfigureHw();

	std::cout << "\n*** Changing Value of VCth... ***\n" << std::endl;

	cSystemController.fCbcInterface->WriteCbcReg( cSystemController.fShelveVector[0]->getBoard( 0 )->getModule( 0 )->getCbc( 0 ), "VCth", 0x03 );
	cSystemController.fCbcInterface->WriteCbcReg( cSystemController.fShelveVector[0]->getBoard( 0 )->getModule( 0 )->getCbc( 1 ), "VCth", 0x10 );
	cSystemController.fCbcInterface->ReadAllCbc( cSystemController.fShelveVector[0]->getBoard( 0 )->getModule( 0 ) );

	std::cout << "\n*** Value of VCth Changed ! ***\n" << std::endl;

	cSystemController.fBeBoardInterface->ReadBoardReg( cSystemController.fShelveVector[0]->getBoard( 0 ), EXT_TRG );
	cSystemController.fBeBoardInterface->ReadBoardReg( cSystemController.fShelveVector[0]->getBoard( 0 ), FAKE_DATA );

	uint32_t dump1 = cSystemController.fShelveVector[0]->getBoard( 0 )->getReg( EXT_TRG );
	uint32_t dump2 = cSystemController.fShelveVector[0]->getBoard( 0 )->getReg( FAKE_DATA );

	std::cout << "\nThe value for EXT_TRG is " << dump1 << std::endl;
	std::cout << "\nThe value for FAKE_DATA is " << dump2 << std::endl;

	cSystemController.fShelveVector[0]->getBoard( 0 )->getModule( 0 )->getCbc( 0 )->saveRegMap( "output/output_00.txt" );
	cSystemController.fShelveVector[0]->getBoard( 0 )->getModule( 0 )->getCbc( 1 )->saveRegMap( "output/output_01.txt" );

	std::cout << "*** End of the test programm" << std::endl;

	return 0;
}
