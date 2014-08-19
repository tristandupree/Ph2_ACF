/*!

	\file 				FrontEndDescription.h
	\brief 				FrontEndDescription base class to describe all parameters common to all FE Components in the DAQ chain
	\author 			Lorenzo BIDEGAIN
	\version 			1.0
	\date 				25/06/14
	Support : 			mail to : lorenzo.bidegain@gmail.com

*/


#ifndef FrontEndDescription_h__
#define FrontEndDescription_h__

#include <boost/cstdint.hpp>

/*!
* \namespace Ph2_HwDescription
* \brief Namespace regrouping all the hardware description
*/
namespace Ph2_HwDescription{

	/*!
	* \class FrontEndDescription
	* \brief Describe all parameters common to all FE Components in the DAQ chain
	*/
	class FrontEndDescription {

	public:

		// METHODS

		// 3 C'tors with different parameter sets
		FrontEndDescription( uint8_t pShelveId, uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId, bool pStatus=true );
		FrontEndDescription( uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId );
		FrontEndDescription( );

		//Copy C'tors
		FrontEndDescription(const FrontEndDescription& pFeDesc);

		// Default D'tor
		virtual ~FrontEndDescription( );

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
		*/
		 virtual void setShelveId( uint8_t pShelveId );
		/*!
		* \brief Set the Be ID
		* \param pBeId
		*/
		 virtual void setBeId( uint8_t pBeId );
		/*!
		* \brief Set the FMC ID
		* \param pFMCId
		*/
		 virtual void setFMCId(uint8_t pFMCId);
		/*!
		* \brief Set the FE ID
		* \param pFeId
		*/
		 virtual void setFeId( uint8_t pFeId );
		/*!
		* \brief Set the status
		* \param pStatus
		*/
		 virtual void setStatus (bool pStatus );

	protected:
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

	};

}


#endif
