/*!
*
* \file BeBoardInterface.cc
* \brief BeBoardInterface User interface for the board
* \author Lorenzo BIDEGAIN, Nicolas Pierre
* \date 31/07/14
*
* Support : mail to : lorenzo.bidegain@cern.ch, nicolas.pierre@cern.ch
*
*/

#include "BeBoardInterface.h"

namespace Ph2_HwInterface{

	BeBoardInterface::BeBoardInterface(BeBoardFWMap& pBoardMap):fBoardMap(pBoardMap)
	{
		fBoardFW=NULL;
		prevBoardId=255;
	}

	BeBoardInterface::~BeBoardInterface()
	{

	}

	void BeBoardInterface::setBoard(uint8_t pBoardId)
	{
		if(prevBoardId!=pBoardId)
		{
			BeBoardFWMap::iterator i;
			i=fBoardMap.find(pBoardId);
			if (i==fBoardMap.end())
			{
				std::cout<<"The Board: "<<pBoardId<<"doesn't exist"<<std::endl;
			}
			else
			{
				fBoardFW=i->second;
				prevBoardId=pBoardId;
			}
		}
	}

	void BeBoardInterface::WriteBoardReg(BeBoard* pBoard,const std::string& pRegNode,const uint32_t& pVal)
    {
		setBoard(pBoard->getBeId());

    	fBoardFW->WriteReg( pRegNode,pVal );
    	pBoard->setReg( pRegNode,pVal );
    }


    void BeBoardInterface::ReadBoardReg(BeBoard* pBoard,const std::string& pRegNode)
    {
		setBoard(pBoard->getBeId());

        pBoard->setReg(pRegNode,uint32_t(fBoardFW->ReadReg(pRegNode)));
	}

}
