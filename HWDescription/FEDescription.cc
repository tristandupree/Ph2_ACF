#include "FEDescription.h"

// Implementation of the base class FEDescription to describe the basic properties and connections of each FE component

namespace Ph2_HwDescription{

	FEDescription::FEDescription( UInt_t pShelveId, UInt_t pBeId, UInt_t pFMCId, UInt_t pFeId, bool pStatus ) : 
	fShelveId( pShelveId ),
	fBeId( pBeId ),
	fFMCId( pFMCId ),
	fFeId( pFeId ),
	fStatus( pStatus ){};

	FEDescription::FEDescription( UInt_t pBeId, UInt_t pFMCId, UInt_t pFeId ) : 
	fShelveId( 0 ),
	fBeId( pBeId ),
	fFMCId( pFMCId ),
	fFeId( pFeId ),
	fStatus( true ){};

	FEDescription::FEDescription( ) : 
	fShelveId( 0 ),
	fBeId( 0 ),
	fFMCId( 0 ),
	fFeId( 0 ),
	fStatus( true) {};

	FEDescription::FEDescription(FEDescription& pFeDesc)
	{
		fShelveId=pFeDesc.fShelveId;
		fBeId=pFeDesc.fBeId;
		fFMCId=pFeDesc.fFMCId;
		fFeId=pFeDesc.fFeId;
		fStatus=pFeDesc.fStatus;
	}

	FEDescription::~FEDescription(){};

	// SetterMethods

	// Shelve Id
	UInt_t FEDescription::setShelveId( UInt_t pShelveId ){
		fShelveId = pShelveId;
		return fShelveId;
	}

	// BIO Board Id
	UInt_t FEDescription::setBeId( UInt_t pBeId ){
		fBeId = pBeId;
		return fBeId;
	}

	// BIO Board FMC Connector Id
	UInt_t FEDescription::setFMCId( UInt_t pFMCId ){
		fFMCId = pFMCId;
		return fFMCId;
	}

	// Fe Id
	UInt_t FEDescription::setFeId( UInt_t pFeId ){
		fFeId = pFeId;
		return fFeId;
	}

	// Status
	bool FEDescription::setStatus( bool pStatus ){
		fStatus = pStatus;
		return fStatus;
	}

}
