#ifndef _CbcRegItem_h__
#define _CbcRegItem_h__

#include "Rtypes.h"

namespace Ph2_HwDescription{

	// Struct for CbcRegisterItem that is identified by (Name), Page, Address, DefaultValue, Value
	struct CbcRegItem{

		/* Keep the Name?  Lorenzo: I'm not sure it's necessary to keep the Name here because the variable structure will be
		already associated with the key of the CbcRegMap wich is already the Name */

		UInt_t fPage;
		UInt_t fAddress;
		UInt_t fDefValue;
		UInt_t fValue;

	};

	// Functor to compare CbcRegItems
	// Do we need this? If we keep the name it might be UInt_teresting to use a set instead of std::map and do a find?
	//Lorenzo: i think it's more UInt_teresting to keep std::map, it has already a method find
	struct RegItemComparer {

	    bool operator() (const CbcRegItem &ri1, const CbcRegItem &ri2) const {
	    	// return ri1.fName < ri2.fName;
	        if( ri1.fPage == ri2.fPage ) return ri1.fAddress < ri2.fAddress;
	        else return ri1.fPage < ri2.fPage;
	    }


};

}

#endif
