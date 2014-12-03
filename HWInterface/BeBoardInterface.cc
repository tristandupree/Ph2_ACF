/*

        FileName :                    BeBoardInterface.cc
        Content :                     User Interface to the Boards
        Programmer :                  Lorenzo BIDEGAIN, Nicolas PIERRE
        Version :                     1.0
        Date of creation :            31/07/14
        Support :                     mail to : lorenzo.bidegain@gmail.com nico.pierre@icloud.com

 */

#include "BeBoardInterface.h"

namespace Ph2_HwInterface
{

	BeBoardInterface::BeBoardInterface( const BeBoardFWMap& pBoardMap ) :
		fBoardMap( pBoardMap ),
		fBoardFW( nullptr ),
		prevBoardIdentifier( 65535 )
	{
	}

	BeBoardInterface::~BeBoardInterface()
	{

	}

	void BeBoardInterface::setBoard( uint16_t pBoardIdentifier )
	{
		if ( prevBoardIdentifier != pBoardIdentifier )
		{
			BeBoardFWMap::iterator i = fBoardMap.find( pBoardIdentifier );
			if ( i == fBoardMap.end() )
				std::cout << "The Board: " << +( pBoardIdentifier >> 8 ) <<  " in Shelve " << +( static_cast<uint8_t>( pBoardIdentifier ) ) << "  doesn't exist" << std::endl;
			else
			{
				fBoardFW = i->second;
				prevBoardIdentifier = pBoardIdentifier;
			}
		}
	}

	void BeBoardInterface::WriteBoardReg( BeBoard* pBoard, const std::string& pRegNode, const uint32_t& pVal )
	{
		setBoard( pBoard->getBeBoardIdentifier() );

		fBoardFW->WriteReg( pRegNode, pVal );
		pBoard->setReg( pRegNode, pVal );
	}


	void BeBoardInterface::ReadBoardReg( BeBoard* pBoard, const std::string& pRegNode )
	{
		setBoard( pBoard->getBeBoardIdentifier() );

		pBoard->setReg( pRegNode, static_cast<uint32_t>( fBoardFW->ReadReg( pRegNode ) ) );
	}


	void BeBoardInterface::getBoardInfo( const BeBoard* pBoard )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->getBoardInfo();
	}

	void BeBoardInterface::ConfigureBoard( const BeBoard* pBoard )
	{
		setBoard( pBoard->getBeId() );
		fBoardFW->ConfigureBoard( pBoard );
	}


	void BeBoardInterface::Start( BeBoard* pBoard )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->Start();
	}


	void BeBoardInterface::Stop( BeBoard* pBoard, uint32_t pNthAcq )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->Stop( pNthAcq );
	}


	void BeBoardInterface::Pause( BeBoard* pBoard )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->Pause();
	}


	void BeBoardInterface::Resume( BeBoard* pBoard )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->Resume();
	}


	void BeBoardInterface::ReadData( BeBoard* pBoard, uint32_t pNthAcq, bool pBreakTrigger )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->ReadData( pBoard, pNthAcq, pBreakTrigger );
	}

	const Event* BeBoardInterface::GetNextEvent( const BeBoard* pBoard )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->GetNextEvent( pBoard );
	}

	const char* BeBoardInterface::GetBuffer( const BeBoard* pBoard, uint32_t& pBufSize )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->GetBuffer( pBufSize );
	}

}
