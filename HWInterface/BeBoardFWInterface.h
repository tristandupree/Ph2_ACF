/*!
*
* \file BeBoardFWInterface.h
* \brief BeBoardFWInterface base class of all type of boards
* \author Lorenzo BIDEGAIN, Nicolas Pierre
* \date 28/07/14
*
* Support : mail to : lorenzo.bidegain@cern.ch, nicolas.pierre@cern.ch
*
*/

#ifndef __BEBOARDFWINTERFACE_H__
#define __BEBOARDFWINTERFACE_H__

#include <boost/format.hpp>
#include <uhal/uhal.hpp>
#include "RegManager.h"
#include "Event.h"
#include "Data.h"
#include "Utilities.h"
#include "Exception.h"
#include "../HWDescription/BeBoard.h"
#include "../HWDescription/Definition.h"
#include "../HWDescription/CbcRegItem.h"
#include "../HWDescription/Cbc.h"
#include "../HWDescription/Module.h"
#include "../HWDescription/BeBoard.h"

using namespace Ph2_HwDescription;

/*!
* \namespace Ph2_HwInterface
* \brief Namespace regrouping all the interfaces to the hardware
*/
namespace Ph2_HwInterface
{
	/*!
	* \class BeBoardFWInterface
	* \brief Class separating board system FW interface from uHal wrapper
	*/
	class BeBoardFWInterface : public RegManager
	{

		public:
			unsigned int fNTotalAcq;
	        bool fStop;

			std::ofstream *fDataFile; /*!< File storing data*/
	        Data *fData; /*!< Data read storage*/

		public:

			/*!
			* \brief Constructor of the BeBoardFWInterface class
			* \param puHalConfigFileName : path of the uHal Config File
			*/
			BeBoardFWInterface(const char *puHalConfigFileName, uint32_t pBoardId);
			/*!
			* \brief Destructor of the BeBoardFWInterface class
			*/
			virtual ~BeBoardFWInterface();

			/*!
			* \brief Get the board infos
			* \param pBoard
			*/
			virtual	void defineEventSize(uint32_t cNbCbc);
			/*!
			* \brief Get the board infos
			* \param pBoard
			*/
			virtual	void getBoardInfo(BeBoard* pBoard);

			//These two methods will be implemented soon
			virtual void FlashProm(){};
			virtual void ProgramCdce(){};

			//Encode/Decode Cbc values
			/*!
			* \brief Encode a/several word(s) readable for a Cbc
			* \param pRegItem : RegItem containing infos (name, adress, value...) about the register to write
			* \param pCbcId : Id of the Cbc to work with
			* \param pVecReq : Vector to stack the encoded words
			*/
			virtual void EncodeReg(CbcRegItem& pRegItem, uint8_t& pCbcId, std::vector<uint32_t>& pVecReq); /*!< Encode a/several word(s) readable for a Cbc*/
			/*!
			* \brief Decode a word from a read of a register of the Cbc
	    	* \param pRegItem : RegItem containing infos (name, adress, value...) about the register to read
	    	* \param pCbcId : Id of the Cbc to work with
	    	* \param pWord : variable to put the decoded word
	    	*/
	    	virtual void DecodeReg(CbcRegItem& pRegItem, uint8_t& pCbcId, uint32_t pWord); /*!< Decode a word from a read of a register of the Cbc*/


			//virtual pure methods which are defined in the proper BoardFWInterface class
			//r/w the Cbc registers
	    	/*!
	    	* \brief Write register blocks of a Cbc
	    	* \param pCbc : Cbc to work with
	    	* \param pVecReq : Block of words to write
	    	*/
	    	virtual void WriteCbcBlockReg( uint8_t& pFeId, std::vector<uint32_t>& pVecReq ) {};
	    	/*!
	    	* \brief Read register blocks of a Cbc
	    	* \param pCbc : Cbc to work with
	    	* \param pVecReq : Vector to stack the read words
	    	*/
	    	virtual void ReadCbcBlockReg( uint8_t& pFeId, std::vector<uint32_t>& pVecReq ) {};
			/*!
	    	* \brief Configure the board with its Config File
	    	* \param pBoard
	    	*/
			virtual void ConfigureBoard(BeBoard* pBoard) {};
			/*!
			* \brief Start a DAQ
			* \param pBoard
			*/
			virtual void Start() {};
			/*!
			* \brief Stop a DAQ
			* \param pBoard
			* \param pNthAcq : actual number of acquisitions
			*/
			virtual void Stop(uint32_t pNthAcq) {};
			/*!
			* \brief Pause a DAQ
			* \param pBoard
			*/
			virtual void Pause() {};
			/*!
			* \brief Unpause a DAQ
			* \param pBoard
			*/
			virtual void Resume() {};
			/*!
			* \brief Read data from DAQ
			* \param pBoard
			* \param pNthAcq : actual number of acquisitions
			* \param pBreakTrigger : if true, enable the break trigger
			*/
			virtual void ReadData( Module* cModule, uint32_t pNthAcq, bool pBreakTrigger) {};


	};
}

#endif
