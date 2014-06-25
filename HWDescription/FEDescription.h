/*!
*
* \file FEDescription.h
* \brief FEDescription base class to describe all parameters common to all FE Components in the DAQ chain 
* \author Lorenzo BIDEGAIN
* \date 25/06/14
*
* Support : mail to : lorenzo.bidegain@cern.ch
*
*/


#ifndef FEDescription_h__
#define FEDescription_h__

#include <boost/cstdint.hpp>

/*!
* \namespace Ph2_HwDescription
* \brief Namespace regrouping all the hardware description
*/
namespace Ph2_HwDescription{

	/*!
	* \class FEDescription
	* \brief Describe all parameters common to all FE Components in the DAQ chain
	*/
	class FEDescription {

	public:
		// MEMBERS

		// Crate (uTCA) that the FE is connected to
		uint8_t fShelveId;
		// BIO Board Id that the FE is connected to
		uint8_t fBeId;
		// Id of the FMC Slot on the BIO Board, all FEs need to know so the right FW registers can be written
		uint8_t fFMCId;
		// Id of the FE (module/hybrid, etc...)
		uint8_t fFeId;
		// status (true=active, false=disabled)
		bool fStatus;

		// METHODS

		// 3 C'tors with different parameter sets
		FEDescription( uint8_t pShelveId, uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId, bool pStatus=true );
		FEDescription( uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId );
		FEDescription( );

		//Copy C'tors
		FEDescription(const FEDescription& pFeDesc);

		// Default D'tor
		virtual ~FEDescription( );

		// Getter methods

		/*!
		* \brief Get the Shelve ID
		* \return The Shelve ID
		*/
		 virtual uint8_t getShelveId()
		{return fShelveId;};

		/*!
		* \brief Get the Be ID
		* \return The Be ID
		*/
		 virtual uint8_t getBeId()
		{ return fBeId ;};

		/*!
		* \brief Get the FMC ID
		* \return The FMC ID
		*/
		 virtual uint8_t getFMCId()
		{ return fFMCId;};

		/*!
		* \brief Get the FE ID
		* \return The FE ID
		*/
		 virtual uint8_t getFeId()
		{ return fFeId ;};

		/*!
		* \brief Get the Status
		* \return The Status
		*/
		 virtual bool getStatus()
		{ return fStatus ;};

		// Setter methods
		
		/*!
		* \brief Set the Shelve ID
		* \param pShelveId
		* \return the Shelve ID
		*/
		 virtual uint8_t setShelveId( uint8_t pShelveId );
		/*!
		* \brief Set the Be ID
		* \param pBeId
		* \return the Be ID
		*/
		 virtual uint8_t setBeId( uint8_t pBeId );
		/*!
		* \brief Set the FMC ID
		* \param pFMCId
		* \return the FMC ID
		*/
		 virtual uint8_t setFMCId(uint8_t pFMCId);
		/*!
		* \brief Set the FE ID
		* \param pFeId
		* \return the Fe ID
		*/
		 virtual uint8_t setFeId( uint8_t pFeId );
		/*!
		* \brief Set the status
		* \param pStatus
		* \return the Status
		*/
		 virtual bool setStatus (bool pStatus );
	};

}


#endif
