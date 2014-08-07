/*

	FileName :                    BeBoardInterface.cc
	Content :                     User Interface to the Boards
	Programmer :                  Lorenzo BIDEGAIN, Nicolas Pierre
	Version :                     0.3
	Date of creation :            31/07/14
	Support :                     mail to : nico.pierre@icloud.com

*/

#include "BeBoardInterface.h"

namespace Ph2_HwInterface{

	BeBoardInterface::BeBoardInterface(BeBoardFWMap& pBoardMap):
		fBoardMap(pBoardMap)
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


	void BeBoardInterface::getBoardInfo(BeBoard* pBoard)
	{
		setBoard(pBoard->getBeId());
		fBoardFW->getBoardInfo();
	}

	void BeBoardInterface::ConfigureBoard(BeBoard* pBoard)
	{
		setBoard(pBoard->getBeId());
		fBoardFW->ConfigureBoard(pBoard);
	}


	void BeBoardInterface::Start(BeBoard* pBoard)
	{
		setBoard(pBoard->getBeId());
		fBoardFW->Start();
	}


	void BeBoardInterface::Stop(BeBoard* pBoard,uint32_t pNthAcq)
	{
		setBoard(pBoard->getBeId());
		fBoardFW->Stop(pNthAcq);
	}


	void BeBoardInterface::Pause(BeBoard* pBoard)
	{
		setBoard(pBoard->getBeId());
		fBoardFW->Pause();
	}


	void BeBoardInterface::Resume(BeBoard* pBoard)
	{
		setBoard(pBoard->getBeId());
		fBoardFW->Resume();
	}


	void BeBoardInterface::ReadData(BeBoard* pBoard, uint32_t pNthAcq, bool pBreakTrigger)
	{
		setBoard(pBoard->getBeId());
		fBoardFW->ReadData(pBoard, pNthAcq, pBreakTrigger);
	}

}
