#include "FEDescription.h"

// Implementation of the base class FEDescription to describe the basic properties and connections of each FE component

namespace Ph2_HwDescription{

	FEDescription::FEDescription( uint8_t pShelveId, uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId, bool pStatus ) : 
	fShelveId( pShelveId ),
	fBeId( pBeId ),
	fFMCId( pFMCId ),
	fFeId( pFeId ),
	fStatus( pStatus ){};

	FEDescription::FEDescription( uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId ) : 
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

	FEDescription::FEDescription(const FEDescription& pFeDesc)
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
	uint8_t FEDescription::setShelveId( uint8_t pShelveId ){
		fShelveId = pShelveId;
		return fShelveId;
	}

	// BIO Board Id
	uint8_t FEDescription::setBeId( uint8_t pBeId ){
		fBeId = pBeId;
		return fBeId;
	}

	// BIO Board FMC Connector Id
	uint8_t FEDescription::setFMCId( uint8_t pFMCId ){
		fFMCId = pFMCId;
		return fFMCId;
	}

	// Fe Id
	uint8_t FEDescription::setFeId( uint8_t pFeId ){
		fFeId = pFeId;
		return fFeId;
	}

	// Status
	bool FEDescription::setStatus( bool pStatus ){
		fStatus = pStatus;
		return fStatus;
	}

}
