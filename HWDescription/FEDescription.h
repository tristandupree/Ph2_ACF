#ifndef FEDescription_h__
#define FEDescription_h__

#include "Rtypes.h"

// Base Class to describe all parameters common to all FE Components in the DAQ chain
// mainly connections & staus for the moment
// G. Auzinger <georg.auzinger@cern.ch>, 04.06.2014

namespace Ph2_HwDescription{

	class FEDescription {

	public:
		// MEMBERS

		// Crate (uTCA) that the FE is connected to
		uint32_t fShelveId;
		// BIO Board Id that the FE is connected to
		uint32_t fBeId;
		// Id of the FMC Slot on the BIO Board, all FEs need to know so the right FW registers can be written
		uint32_t fFMCId;
		// Id of the FE (module/hybrid, etc...)
		uint32_t fFeId;
		// status (true=active, false=disabled)
		bool fStatus;

		// METHODS

		// 3 C'tors with different parameter sets
		FEDescription( uint32_t pShelveId, uint32_t pBeId, uint32_t pFMCId, uint32_t pFeId, bool pStatus=true );
		FEDescription( uint32_t pBeId, uint32_t pFMCId, uint32_t pFeId );
		FEDescription( );

		//Copy C'tors
		FEDescription(const FEDescription& pFeDesc);

		// Default D'tor
		virtual ~FEDescription( );

		// Getter methods
		 virtual uint32_t getShelveId()
		{return fShelveId;};

		 virtual uint32_t getBeId()
		{ return fBeId ;};

		 virtual uint32_t getFMCId()
		{ return fFMCId;};

		 virtual uint32_t getFeId()
		{ return fFeId ;};

		 virtual bool getStatus()
		{ return fStatus ;};

		// Setter methods
		 virtual uint32_t setShelveId( uint32_t pShelveId );
		 virtual uint32_t setBeId( uint32_t pBeId );
		 virtual uint32_t setFMCId(uint32_t pFMCId);
		 virtual uint32_t setFeId( uint32_t pFeId );
		 virtual bool setStatus (bool pStatus );
	};

}


#endif
