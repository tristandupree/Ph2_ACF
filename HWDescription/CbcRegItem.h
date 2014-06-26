/*!
*
* \file CbcRegItem.h
* \brief CbcRegItem description, contents of the structure CbcRegItem with is the value of the CbcRegMap
* \author Lorenzo BIDEGAIN
* \date 25/06/14
*
* Support : mail to : lorenzo.bidegain@cern.ch
*
*/

#ifndef _CbcRegItem_h__
#define _CbcRegItem_h__

#include <boost/cstdint.hpp>

namespace Ph2_HwDescription{

	/*!
	* \struct CbcRegItem
	* \brief Struct for CbcRegisterItem that is identified by (Name), Page, Address, DefaultValue, Value
	*/
	struct CbcRegItem{

		/* Keep the Name?  Lorenzo: I'm not sure it's necessary to keep the Name here because the variable structure will be
		already associated with the key of the CbcRegMap wich is already the Name */

		uint8_t fPage;
		uint8_t fAddress;
		uint8_t fDefValue;
		uint8_t fValue;

	};

	// Functor to compare CbcRegItems
	// Do we need this? If we keep the name it might be uint8_teresting to use a set instead of std::map and do a find?
	//Lorenzo: i think it's more uint8_teresting to keep std::map, it has already a method find

	//Put in comment by Nicolas 20/06, useless now
	/*
	struct RegItemComparer {

	    bool operator() (const CbcRegItem &ri1, const CbcRegItem &ri2) const {
	    	// return ri1.fName < ri2.fName;
	        if( ri1.fPage == ri2.fPage ) return ri1.fAddress < ri2.fAddress;
	        else return ri1.fPage < ri2.fPage;
	    }


};
*/
}

#endif
