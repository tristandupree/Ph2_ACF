/*!
*
* \file Glib.cc
* \brief Glib Description class, configs of the Glib
* \author Lorenzo BIDEGAIN
* \date 25/06/14
*
* Support : mail to : lorenzo.bidegain@cern.ch
*
*/


#include "Glib.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

namespace Ph2_HwDescription{

	// Constructors
	Glib::Glib():BeBoard( 0, 0, 0, DEFAULT_GLIB_FILE ){
	}

	Glib::Glib( uint8_t pShelveId, uint8_t pBeId, uint8_t pNFe, std::string filename ):BeBoard( pShelveId, pBeId, pNFe, filename){
	}

	Glib::Glib( uint8_t pShelveId, uint8_t pBeId, uint8_t pNFe, /*uint8_t pFMCConfiguration,*/ bool pExtTrg, bool pFakeData ,  std::string filename):BeBoard( pShelveId, pBeId, pNFe, filename)
	{
		//FMCConfiguration are not yet in the registers of the firmware
		//fRegMap["FMCConfiguration"]=pFMCConfiguration;
		fRegMap[EXT_TRG]=pExtTrg;
		fRegMap[FAKE_DATA]=pFakeData;

	}


}
