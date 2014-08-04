/*!
*
* \file BeBoardFWInterface.cc
* \brief BeBoardFWInterface base class of all type of boards
* \author Lorenzo BIDEGAIN, Nicolas Pierre
* \date 28/07/14
*
* Support : mail to : lorenzo.bidegain@cern.ch, nicolas.pierre@cern.ch
*
*/

#define DEV_FLAG         0

#include "BeBoardFWInterface.h"

namespace Ph2_HwInterface
{

	//Constructor, makes the board map
	BeBoardFWInterface::BeBoardFWInterface(const char *puHalConfigFileName):
        RegManager(puHalConfigFileName),
        fNTotalAcq(0),
        fDataFile(0),
	//fNegativeLogicCBC(true),
	fStop(false)
	{
		fData = new Data();
	}


	BeBoardFWInterface::~BeBoardFWInterface()
	{
		delete fData;
    	}


	void BeBoardFWInterface::getBoardInfo(Glib& pGlib)
	{
        	std::cout << "FMC1 present : " << uint32_t(ReadReg(FMC1_PRESENT)) << std::endl;
        	std::cout << "FMC2 present : " << uint32_t(ReadReg(FMC2_PRESENT)) << std::endl;
        	std::cout << "FW version : " << uint32_t(ReadReg(FW_VERSION_MAJOR)) << "." << uint32_t(ReadReg(FW_VERSION_MINOR)) << "." << uint32_t(ReadReg(FW_VERSION_BUILD)) << std::endl;

        	uhal::ValWord<uint32_t> cBoardType = ReadReg(BOARD_TYPE);

        	uint32_t cMask(0x00000000);
        	unsigned int i(0);
        	char cChar;

        	std::cout << "BoardType : ";

        	for( i=24; i < 32; i++ ) cMask |= ( (uint32_t) 1 << i );
        	cChar = ( ( cBoardType & cMask ) >> 24 );

        	std::cout << cChar;

        	for( cMask=0, i=16; i < 24; i++ ) cMask |= (uint32_t) 1 << i;
        	cChar = (( cBoardType & cMask ) >> 16);

        	std::cout << cChar;

        	for( cMask=0, i=8; i < 16; i++ ) cMask |= (uint32_t) 1 << i;
        	cChar = (( cBoardType & cMask ) >> 8);

        	std::cout << cChar;

        	for( cMask=0, i=0; i < 8; i++ ) cMask |= (uint32_t) 1 << i;
        	cChar = (cBoardType & cMask);

        	std::cout << cChar << std::endl;

        	std::cout << "FMC User Board ID : " << uint32_t(ReadReg(FMC_USER_BOARD_ID)) << std::endl;
        	std::cout << "FMC User System ID : " << uint32_t(ReadReg(FMC_USER_SYS_ID)) << std::endl;
        	std::cout << "FMC User Version : " << uint32_t(ReadReg(FMC_USER_VERSION)) << std::endl;

	}


	void BeBoardFWInterface::EncodeReg(CbcRegItem& pRegItem, uint8_t& pCbcId, std::vector<uint32_t>& pVecReq)
	{
		pVecReq.push_back(pCbcId<<17 | pRegItem.fPage<<16 | pRegItem.fAddress<<8 | pRegItem.fValue);
	}


	void BeBoardFWInterface::DecodeReg(CbcRegItem& pRegItem, uint8_t& pCbcId, uint32_t pWord)
	{
		uint32_t cMask(0x00000000);
		unsigned int i(0);

		for( i=17; i < 21; i++ ) cMask |= ( (uint32_t) 1 << i );
		pCbcId = ( ( pWord & cMask ) >> 17 );

		for( cMask=0, i=16; i < 17; i++ ) cMask |= (uint32_t) 1 << i;
		pRegItem.fPage = ( pWord & cMask ) >> 16;

		for( cMask=0, i=8; i < 16; i++ ) cMask |= (uint32_t) 1 << i;
		pRegItem.fAddress = ( pWord & cMask ) >> 8;

		for( cMask=0, i=0; i < 8; i++ ) cMask |= (uint32_t) 1 << i;
		pRegItem.fValue = pWord & cMask;
	}

}
