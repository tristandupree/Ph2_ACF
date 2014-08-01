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
		if(prevBoardId==pBoardId)
			return;
		else
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

	void BeBoardInterface::WriteGlibReg(Glib& pGlib,const std::string& pRegNode,const uint32_t& pVal)
    	{
		setBoard(pGlib.getBeId());
        	fBoardFW->ChooseBoard(pGlib.getBeId());

        	fBoardFW->WriteReg( pRegNode,pVal );
        	pGlib.setReg( pRegNode,pVal );
    	}


    	void BeBoardInterface::ReadGlibReg(Glib& pGlib,const std::string& pRegNode)
    	{
		setBoard(pGlib.getBeId());
        	fBoardFW->ChooseBoard(pGlib.getBeId());

        	pGlib.setReg(pRegNode,(uint32_t) fBoardFW->ReadReg(pRegNode));
	}

}
