#ifndef Module_h__
#define Module_h__

#include "FEDescription.h"
#include "Cbc.h"
#include <vector>

// FE Hybrid HW Description Class

namespace Ph2_HwDescription{

	class Module : public FEDescription{
	
	public:

		// C'tors take FEDescription or hierachy of connection and the # of CBCs
		Module( FEDescription& pFeDesc );
		Module ( uint8_t pShelveId, uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId);

		// Default C'tor
		Module();

		// D'tor
		~Module(){};

		uint8_t getNCbc(){return fCbcVector.size();};
		void addCbc( Cbc& pCbc );
		bool   removeCbc( uint8_t pCbcId );
		Cbc&   getCbc( uint8_t pCbcId );

	protected:

		std::vector < Cbc > fCbcVector;

		// This is really all this class needs at the moment, connection and status are already included in the FEDescription parent class!

	};
}


#endif
