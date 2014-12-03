/*!

        \file                                            BeBoardInterface.h
        \brief                                           User Interface to the Boards
        \author                                          Lorenzo BIDEGAIN, Nicolas PIERRE
        \version                                         1.0
        \date                        31/07/14
        Support :                    mail to : lorenzo.bidegain@gmail.com, nicolas.pierre@cern.ch

 */

#ifndef __BEBOARDINTERFACE_H__
#define __BEBOARDINTERFACE_H__

#include "BeBoardFWInterface.h"

using namespace Ph2_HwDescription;

/*!
 * \namespace Ph2_HwInterface
 * \brief Namespace regrouping all the interfaces to the hardware
 */
namespace Ph2_HwInterface
{

	typedef std::map<uint16_t, BeBoardFWInterface*> BeBoardFWMap;    /*!< Map of Board connected */

	/*!
	 * \class BeBoardInterface
	 * \brief Class representing the User Interface to the different boards
	 */
	class BeBoardInterface
	{

	  private:
		BeBoardFWMap fBoardMap;                     /*!< Map of Board connected */
		BeBoardFWInterface* fBoardFW;                     /*!< Board loaded */
		uint16_t prevBoardIdentifier;                     /*!< Id of the previous board */

	  private:
		/*!
		 * \brief Set the board to talk with
		 * \param pBoardId
		 */
		void setBoard( uint16_t pBoardIdentifier );

	  public:
		/*!
		 * \brief Constructor of the BeBoardInterface class
		 * \param Reference to the BoardFWInterface
		 */
		BeBoardInterface( const BeBoardFWMap& pBoardMap );
		/*!
		 * \brief Destructor of the BeBoardInterface class
		 */
		~BeBoardInterface();

		/*!
		 * \brief Update both Board register and Config File
		 * \param pBoard
		 * \param pRegNode : Node of the register to update
		 * \param pVal : Value to write
		 */
		void WriteBoardReg( BeBoard* pBoard, const std::string& pRegNode, const uint32_t& pVal );
		/*!
		 * \brief Update Config File with the value in the Board register
		 * \param pBoard
		 * \param pRegNode : Node of the register to update
		 */
		void ReadBoardReg( BeBoard* pBoard, const std::string& pRegNode );

		/*!
		 * \brief Get the board infos
		 * \param pBoard
		 */
		void getBoardInfo( const BeBoard* pBoard );

		/*!
		 * \brief Configure the board with its Config File
		 * \param pBoard
		 */
		void ConfigureBoard( const BeBoard* pBoard );
		/*!
		 * \brief Start a DAQ
		 * \param pBoard
		 */
		void Start( BeBoard* pBoard );
		/*!
		 * \brief Stop a DAQ
		 * \param pBoard
		 * \param pNthAcq : actual number of acquisitions
		 */
		void Stop( BeBoard* pBoard, uint32_t pNthAcq );
		/*!
		 * \brief Pause a DAQ
		 * \param pBoard
		 */
		void Pause( BeBoard* pBoard );
		/*!
		 * \brief Resume a DAQ
		 * \param pBoard
		 */
		void Resume( BeBoard* pBoard );
		/*!
		 * \brief Read data from DAQ
		 * \param pBoard
		 * \param pNthAcq : actual number of acquisitions
		 * \param pBreakTrigger : if true, enable the break trigger
		 */
		void ReadData( BeBoard* pBoard, uint32_t pNthAcq, bool pBreakTrigger );
		/*!
		 * \brief Get next event from data buffer
		 * \param pBoard
		 * \return Next event
		 */
		const Event* GetNextEvent( const BeBoard* pBoard );
		/*!
		 * \brief Get the data buffer
		 * \param pBufSize : recovers the data buffer size
		 * \return Data buffer
		 */
		const char* GetBuffer( const BeBoard* pBeBoard, uint32_t& pBufSize );

	};
}

#endif
