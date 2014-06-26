/*!
*
* \file FrontEndDescription.cc
* \brief FrontEndDescription base class to describe all parameters common to all FE Components in the DAQ chain 
* \author Lorenzo BIDEGAIN
* \date 25/06/14
*
* Support : mail to : lorenzo.bidegain@cern.ch
*
*/

#include "FrontEndDescription.h"

// Implementation of the base class FrontEndDescription to describe the basic properties and connections of each FE component

namespace Ph2_HwDescription{

	FrontEndDescription::FrontEndDescription( uint8_t pShelveId, uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId, bool pStatus ) : 
	fShelveId( pShelveId ),
	fBeId( pBeId ),
	fFMCId( pFMCId ),
	fFeId( pFeId ),
	fStatus( pStatus ){};

	FrontEndDescription::FrontEndDescription( uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId ) : 
	fShelveId( 0 ),
	fBeId( pBeId ),
	fFMCId( pFMCId ),
	fFeId( pFeId ),
	fStatus( true ){};

	FrontEndDescription::FrontEndDescription( ) : 
	fShelveId( 0 ),
	fBeId( 0 ),
	fFMCId( 0 ),
	fFeId( 0 ),
	fStatus( true) {};

	FrontEndDescription::FrontEndDescription(const FrontEndDescription& pFeDesc)
	{
		fShelveId=pFeDesc.fShelveId;
		fBeId=pFeDesc.fBeId;
		fFMCId=pFeDesc.fFMCId;
		fFeId=pFeDesc.fFeId;
		fStatus=pFeDesc.fStatus;
	}

	FrontEndDescription::~FrontEndDescription(){};

	// SetterMethods

	// Shelve Id
	uint8_t FrontEndDescription::setShelveId( uint8_t pShelveId ){
		fShelveId = pShelveId;
		return fShelveId;
	}

	// BIO Board Id
	uint8_t FrontEndDescription::setBeId( uint8_t pBeId ){
		fBeId = pBeId;
		return fBeId;
	}

	// BIO Board FMC Connector Id
	uint8_t FrontEndDescription::setFMCId( uint8_t pFMCId ){
		fFMCId = pFMCId;
		return fFMCId;
	}

	// Fe Id
	uint8_t FrontEndDescription::setFeId( uint8_t pFeId ){
		fFeId = pFeId;
		return fFeId;
	}

	// Status
	bool FrontEndDescription::setStatus( bool pStatus ){
		fStatus = pStatus;
		return fStatus;
	}

}
