/*!
*
* \file Glib.h
* \brief Glib Description class, configs of the Glib
* \author Lorenzo BIDEGAIN
* \date 25/06/14
*
* Support : mail to : lorenzo.bidegain@cern.ch
*
*/

#ifndef _Glib_h__
#define _Glib_h__

#include "BeBoard.h"

/*!
* \namespace Ph2_HwDescription
* \brief Namespace regrouping all the hardware description
*/
namespace Ph2_HwDescription{

	

	/*!
	* \class Glib
	* \brief Read/Write Glib's registers on a file, contains a register map and contains a vector of Module which are connected to the Glib
	*/
	class Glib : public BeBoard{

	public:

		// C'tors: the Glib only needs to know about it's shelf and which BE it is + the # of FEs connected
		// C'tor for a standard Glib
		Glib( uint8_t pShelveId, uint8_t pBeId, uint8_t pNFe, std::string filename = DEFAULT_GLIB_FILE );
		// Parameters that define system for us
		//Modify with new param
		Glib( uint8_t pShelveId, uint8_t pBeId, uint8_t pNFe,/* uint8_t pFMCConfiguration,*/ bool pExtTrg, bool pFakeData = false , std::string filename = "default_glib_file" );
		// Default C'tor
		Glib();

		// D'Tor
		~Glib(){};


	};
}

#endif
