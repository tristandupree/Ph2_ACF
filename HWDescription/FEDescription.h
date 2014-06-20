#ifndef FEDescription_h__
#define FEDescription_h__

#include <boost/cstdint.hpp>

// Base Class to describe all parameters common to all FE Components in the DAQ chain
// mainly connections & staus for the moment
// G. Auzinger <georg.auzinger@cern.ch>, 04.06.2014

namespace Ph2_HwDescription{

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
		 virtual uint8_t getShelveId()
		{return fShelveId;};

		 virtual uint8_t getBeId()
		{ return fBeId ;};

		 virtual uint8_t getFMCId()
		{ return fFMCId;};

		 virtual uint8_t getFeId()
		{ return fFeId ;};

		 virtual bool getStatus()
		{ return fStatus ;};

		// Setter methods
		 virtual uint8_t setShelveId( uint8_t pShelveId );
		 virtual uint8_t setBeId( uint8_t pBeId );
		 virtual uint8_t setFMCId(uint8_t pFMCId);
		 virtual uint8_t setFeId( uint8_t pFeId );
		 virtual bool setStatus (bool pStatus );
	};

}


#endif
