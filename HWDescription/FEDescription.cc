#include "FEDescription.h"

// Implementation of the base class FEDescription to describe the basic properties and connections of each FE component

namespace HWDescription{

	FEDescription::FEDescription( int pShelveId, int pBeId, int pFMCId, int pFeId, bool pStatus ) : 
	fShelveId( pShelveId ),
	fBeId( pBeId ),
	fFMCId( pFMCId ),
	fFeId( pFeId ),
	fStatus( pStatus ){};

	FEDescription::FEDescription( int pBeId, int pFMCId, int pFeId ) : 
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
	int FEDescription::setShelveId( int pShelveId ){
		fShelveId = pShelveId;
		return fShelveId;
	}

	// BIO Board Id
	int FEDescription::setBeId( int pBeId ){
		fBeId = pBeId;
		return fBeId;
	}

	// BIO Board FMC Connector Id
	int FEDescription::setFMCId( int pFMCId ){
		fFMCId = pFMCId;
		return fFMCId;
	}

	// Fe Id
	int FEDescription::setFeId( int pFeId ){
		fFeId = pFeId;
		return fFeId;
	}

	// Status
	bool FEDescription::setStatus( bool pStatus ){
		fStatus = pStatus;
		return fStatus;
	}

}
