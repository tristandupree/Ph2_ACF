#ifndef Module_h__
#define Module_h__

#include "HWDescription/FEDescription.h"
#include "Cbc.h"

// FE Hybrid HW Description Class

namespace Ph2_HwDescription{

	class Module : public FEDescription{
	
	public:

		// C'tors take FEDescription or hierachy of connection and the # of CBCs
		Module( FEDescription pFeDesc, UInt_t pNCbc );
		Module ( UInt_t pShelveId, UInt_t pBeId, UInt_t pFMCId, UInt_t pFeId, UInt_t pNCbc );

		// Default C'tor
		Module();

		// D'tor
		~Module(){};

		void   addCbc( Cbc pCbc );
		UInt_t getNCbc( return fCbcVector.size() );

	protected:

		std::vector < Cbc, CbcComparer > fCbcVector;

		// This is really all this class needs at the moment, connection and status are already included in the FEDescription parent class!

	};
}


#endif