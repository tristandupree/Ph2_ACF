#ifndef Cbc_h__
#define Cbc_h__

#include "HWDescription/FEDescription.h"

// Cbc2 Chip HW Description Class

namespace HWDescription{

	class Cbc : public FEDescription{

	public:

		
		// C'tors with object FE Description 
		Cbc( FEDescription pFeDesc, UInt_t pCbcId, std::string filename );
		Cbc( FEDescription pFeDesc, UInt_t pCbcId, /*most important parameters*/ );
		Cbc( FEDescription pFeDesc, UInt_t pCbcId );

		// C'tors whith take ShelveID, BeId, FeID, CbcId
		Cbc( UInt_t pShelveId, UInt_t pBeId, UInt_t pFeId, UInt_t pCbcId, std::strin filename );
		Cbc( UInt_t pShelveId, UInt_t pBeId, UInt_t pFeId, UInt_t pCbcId, /*most important parameters*/ );
		Cbc( UInt_t pShelveId, UInt_t pBeId, UInt_t pFeId, UInt_t pCbcId );

		// Default C'tor
		Cbc();

		// D'tor
		~Cbc();


	private:

	protected:

	};
}


#endif