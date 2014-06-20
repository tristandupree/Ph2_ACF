#ifndef Module_h__
#define Module_h__

#include "FEDescription.h"
#include "Cbc.h"
#include <set>

// FE Hybrid HW Description Class

namespace Ph2_HwDescription{

	class Module : public FEDescription{
	
	public:

		// C'tors take FEDescription or hierachy of connection and the # of CBCs
		Module( FEDescription& pFeDesc );
		Module ( uint32_t pShelveId, uint32_t pBeId, uint32_t pFMCId, uint32_t pFeId);

		// Default C'tor
		Module();

		// D'tor
		~Module(){};

		uint32_t getNCbc(){return fCbcVector.size();};
		void addCbc( Cbc& pCbc );
		bool   removeCbc( uint32_t pCbcId );
		Cbc   getCbc( uint32_t pCbcId );

	protected:

		std::set < Cbc, CbcComparer > fCbcVector;

		// This is really all this class needs at the moment, connection and status are already included in the FEDescription parent class!

	};
}


#endif
