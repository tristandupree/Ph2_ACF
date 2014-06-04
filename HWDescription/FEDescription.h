#ifndef FEDescription_h__
#define FEDescription_h__

// Base Class to describe all FE Components in the DAQ chain
// very simple for the time beeing
// G. Auzinger <georg.auzinger@cern.ch>, 04.06.2014

namespace HWDescription{

	class FEDescription {

	public:
		// Crate (uTCA) that the FE is connected to
		UInt_t fShelveId;
		// BIO Board Id that the FE is connected to
		UInt_t fBeId;
		// Id of the FE (module/hybrid, etc...)
		UInt_t fFeId;
		// status (true=active, false=disabled)
		bool fStatus;

		// 3 C'tors with different parameter sets
		virtual FEDescription( UInt_t pShelveId, UInt_t pBeId, UInt_t pFeId, bool pStatus );
		virtual FEDescription( UInt_t pBeId, UInt_t pFeId );
		virtual FEDescription( );

		// Default D'tor
		virtual ~FEDescription( );

		// Getter methods
		virtual UInt_t getShelveId( return fShelveId );
		virtual UInt_t getBeId( return fBeId );
		virtual UInt_t getFeId( return fFeId );
		virtual bool   getStatus( return fStatus );

		// Setter methods
		virtual UInt_t setShelveId( UInt_t pShelveId );
		virtual UInt_t setBeId( UInt_t pBeId );
		virtual UInt_t setFeId( UInt_t pFeId );
		virtual bool   setStatus (bool pStatus );
	};

}


#endif