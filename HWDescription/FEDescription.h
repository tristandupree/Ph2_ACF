#ifndef FEDescription_h__
#define FEDescription_h__

// Base Class to describe all parameters common to all FE Components in the DAQ chain
// mainly connections & staus for the moment
// G. Auzinger <georg.auzinger@cern.ch>, 04.06.2014

namespace Ph2_HwDescription{

	class FEDescription {

	public:
		// MEMBERS

		// Crate (uTCA) that the FE is connected to
		UInt_t fShelveId;
		// BIO Board Id that the FE is connected to
		UInt_t fBeId;
		// Id of the FMC Slot on the BIO Board, all FEs need to know so the right FW registers can be written
		UInt_t fFMCId;
		// Id of the FE (module/hybrid, etc...)
		UInt_t fFeId;
		// status (true=active, false=disabled)
		bool fStatus;

		// METHODS

		// 3 C'tors with different parameter sets
		FEDescription( UInt_t pShelveId, UInt_t pBeId, UInt_t pFMCId, UInt_t pFeId, bool pStatus=true );
		FEDescription( UInt_t pBeId, UInt_t pFMCId, UInt_t pFeId );
		FEDescription( );
		
		//Copy C'tors
		FEDescription(FEDescription& pFeDesc);

		// Default D'tor
		~FEDescription( );

		// Getter methods
		 virtual UInt_t getShelveId()
		{return fShelveId;};
		 
		 virtual UInt_t getBeId()
		{ return fBeId ;};

		 virtual UInt_t getFMCId()
		{ return fFMCId;};

		 virtual UInt_t getFeId()
		{ return fFeId ;};

		 virtual bool getStatus()
		{ return fStatus ;};

		// Setter methods
		 virtual UInt_t setShelveId( UInt_t pShelveId );
		 virtual UInt_t setBeId( UInt_t pBeId );
		 virtual UInt_t setFMCId(UInt_t pFMCId);
		 virtual UInt_t setFeId( UInt_t pFeId );
		 virtual bool setStatus (bool pStatus );
	};

}


#endif
