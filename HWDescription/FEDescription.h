#ifndef FEDescription_h__
#define FEDescription_h__

// Base Class to describe all parameters common to all FE Components in the DAQ chain
// mainly connections & staus for the moment
// G. Auzinger <georg.auzinger@cern.ch>, 04.06.2014

namespace HWDescription{

	class FEDescription {

	public:
		// MEMBERS

		// Crate (uTCA) that the FE is connected to
		int fShelveId;
		// BIO Board Id that the FE is connected to
		int fBeId;
		// Id of the FMC Slot on the BIO Board, all FEs need to know so the right FW registers can be written
		int fFMCId;
		// Id of the FE (module/hybrid, etc...)
		int fFeId;
		// status (true=active, false=disabled)
		bool fStatus;

		// METHODS

		// 3 C'tors with different parameter sets
		FEDescription( int pShelveId, int pBeId, int pFMCId, int pFeId, bool pStatus=true );
		FEDescription( int pBeId, int pFMCId, int pFeId );
		FEDescription( );
		
		//Copy C'tors
		FEDescription(FEDescription& pFeDesc);

		// Default D'tor
		~FEDescription( );

		// Getter methods
		 virtual int getShelveId()
		{return fShelveId;};
		 
		 virtual int getBeId()
		{ return fBeId ;};

		 virtual int getFMCId()
		{ return fFMCId;};

		 virtual int getFeId()
		{ return fFeId ;};

		 virtual bool getStatus()
		{ return fStatus ;};

		// Setter methods
		 virtual int setShelveId( int pShelveId );
		 virtual int setBeId( int pBeId );
		 virtual int setFMCId(int pFMCId);
		 virtual int setFeId( int pFeId );
		 virtual bool setStatus (bool pStatus );
	};

}


#endif
