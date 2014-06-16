#include "Module.h"

namespace Ph2_HwDescription{

	// Default C'tor
	Module::Module():FEDescription(){}

	Module::Module( FEDescription& pFeDesc, UInt_t pNCbc ):FEDescription(pFeDesc){
		fCbcVector.reserve( pNCbc );
	}

	Module::Module( UInt_t pShelveId, UInt_t pBeId, UInt_t pFMCId, UInt_t pFeId, UInt_t pNCbc ):FEDescription( pShelveId, pBeId, pFMCId,pFeId ){
		fCbcVector.reserve( pNCbc );
	}

	Module::addCbc( Cbc pCbc ){
		fCbcVector.push_back( pCbc );
	}
}
