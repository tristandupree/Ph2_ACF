/*!

\file Cbc.h
\brief Cbc Description class, config of the Cbcs
\author Lorenzo BIDEGAIN
\date 25/06/14

Support : mail to : lorenzo.bidegain@cern.ch

*/


#ifndef Cbc_h__
#define Cbc_h__

#include "FEDescription.h"
#include "CbcRegItem.h"
#include <iostream>
#include <map>
#include <string>
#include <boost/cstdint.hpp>

// Cbc2 Chip HW Description Class

/*!
* \namespace Ph2_HwDescription
* \brief Namespace regrouping all the hardware description
*/
namespace Ph2_HwDescription{

	#define default_file "default_file.txt"
	typedef std::map < std::string, CbcRegItem > CbcRegMap;

	/*!
	* \class Cbc
	* \brief Read/Write Cbc's registers on a file
	*/
	class Cbc : public FEDescription{

	public:

		// C'tors with object FE Description
		Cbc( FEDescription& pFeDesc, uint8_t pCbcId, std::string filename );
		Cbc( FEDescription& pFeDesc, uint8_t pCbcId,uint8_t pTriggerLatency,uint8_t pVcth );
		Cbc( FEDescription& pFeDesc, uint8_t pCbcId );

		// C'tors which take ShelveID, BeId, FeID, CbcId
		Cbc( uint8_t pShelveId, uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId, uint8_t pCbcId, std::string filename );
		Cbc( uint8_t pShelveId, uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId, uint8_t pCbcId, uint8_t pTriggerLatency,uint8_t pVcth );
		Cbc( uint8_t pShelveId, uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId, uint8_t pCbcId );

		// Default C'tor
		Cbc();

		// Copy C'tor
		Cbc(const Cbc& cbcobj);

		// D'Tor
		~Cbc();

		 /*!
		* \brief Load RegMap from a file
		* \param filename
		*/
		void loadfRegMap(std::string filename);

		 /*!
		* \brief Get the register TriggerLatency from the Map
		* \return The value of TriggerLantency
		*/
		uint8_t getTriggerLatency();
		 /*!
		* \brief Set the register TriggerLatency of the Map
		* \param pTriggerLatency
		*/
		void setTriggerLatency(uint8_t pTriggerLatency);

		/*!
		* \brief Get the register Vcth from the Map
		* \return The value of Vcth
		*/
		uint8_t getVcth();
		/*!
		* \brief Set the register Vcth of the Map
		* \param psetVcth
		*/
		void setVcth(uint8_t psetVcth);

		/*!
		* \brief Get any register from the Map
		* \param pReg
		* \return The value of the register
		*/
		uint8_t getReg(std::string pReg);
		/*!
		* \brief Set any register of the Map
		* \param pReg
		* \param psetValue
		*/
		void setReg(std::string pReg, uint8_t psetValue);

		/*!
		* \brief Write the registers of the Map in a file
		* \param filename
		*/
		void writeRegValues( std::string filename );

		/*!
		* \brief Get the Cbc Id
		* \return The Cbc ID
		*/
		uint8_t getCbcId() {return fCbcId;};

		/*!
		* \brief Get the Map of the registers
		* \return The map of register
		*/
		CbcRegMap getRegMap() {return fRegMap;};

	protected:

		// Map of Register Name vs. RegisterItem that contains: Page, Address, Default Value, Value
		CbcRegMap fRegMap;

	public:

		uint8_t fCbcId;

	};


	/*!
	* \struct CbcComparer
	* \brief Compare two Cbc by their ID
	*/
	struct CbcComparer{

		bool operator() (const Cbc& cbc1,const Cbc& cbc2);

		};

}

#endif
