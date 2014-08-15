/*!
*
* \file Calibration.h
* \brief Calibration class, calibration of the hardware
* \author Lorenzo BIDEGAIN
* \date 13/08/14
*
* Support : mail to : lorenzo.bidegain@cern.ch
*
*/

#ifndef Calibration_h__
#define Calibration_h__

#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../System/SystemController.h"
#include "Channel.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;
using namespace Ph2_System;

/*!
* \class Calibration
* \brief Read/Write Cbc's registers on a file
*/
class Calibration : public SystemController {

public:
			
	// Default C'tor
	Calibration(){};

	// D'Tor
	~Calibration(){};

	/*!
	* \brief Initialise calibration
	*/
	void Initialise();


private:
		
	std::map < std::string, uint8_t > fSettingsMap;

	TestGroupMap fTestGroupMap;


};

#endif
