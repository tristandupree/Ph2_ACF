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
#include "../HWDescription/Glib.h"
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
	* \brief Permit to seperate board syst FW interface from uHal wrapper
	*/
	class BeBoardFWInterface : public RegManager{

	protected:
		unsigned int fNTotalAcq;
            	bool fStop;

		std::ofstream *fDataFile; /*!< File storing data*/

	public:
            	Data *fData; /*!< Data read storage*/
		
	public:
		
		/*!
		* \brief Constructor of the BeBoardFWInterface class
		* \param puHalConfigFileName : path of the uHal Config File
		*/
		BeBoardFWInterface(const char *puHalConfigFileName);
		/*!
		* \brief Destructor of the BeBoardFWInterface class
		*/
		virtual ~BeBoardFWInterface();

		/*!
            	* \brief Get the board infos
            	* \param pGlib
           	*/
            	virtual	void getBoardInfo(Glib& pGlib);
		
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






		//virtual pur methods which are defined in the proper BoardFWInterface class



		//r/w the Cbc registers
            	/*!
            	* \brief Write register blocks of a Cbc
            	* \param pCbc : Cbc to work with
            	* \param pVecReq : Block of words to write
            	*/
            	virtual void WriteCbcBlockReg( Cbc* pCbc, std::vector<uint32_t>& pVecReq )=0;
            	/*!
            	* \brief Read register blocks of a Cbc
            	* \param pCbc : Cbc to work with
            	* \param pVecReq : Vector to stack the read words
            	*/
            	virtual void ReadCbcBlockReg( Cbc* pCbc, std::vector<uint32_t>& pVecReq )=0;



		/*!
            	* \brief Configure the Glib with its Config File
            	* \param pGlib
            	*/
		virtual void ConfigureGlib(Glib& pGlib)=0;
            	/*!
            	* \brief Start a DAQ
           	* \param pGlib
            	*/
            	virtual void Start(Glib& pGlib)=0;
            	/*!
            	* \brief Stop a DAQ
            	* \param pGlib
            	* \param pNthAcq : actual number of acquisitions
            	*/
            	virtual void Stop(Glib& pGlib,uint32_t pNthAcq)=0;
            	/*!
            	* \brief Pause a DAQ
            	* \param pGlib
            	*/
            	virtual void Pause(Glib& pGlib)=0;
            	/*!
            	* \brief Unpause a DAQ
            	* \param pGlib
            	*/
            	virtual void Unpause(Glib& pGlib)=0;
            	/*!
            	* \brief Read data from DAQ
            	* \param pGlib
            	* \param pNthAcq : actual number of acquisitions
            	* \param pBreakTrigger : if true, enable the break trigger
            	*/
            	virtual void ReadData(Glib& pGlib, uint32_t pNthAcq, bool pBreakTrigger)=0;
		/*!
		* \brief Run a DAQ
    	        * \param pGlib
    	        */
            	virtual void Run(Glib& pGlib)=0;


	};
}

#endif
