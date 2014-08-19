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

	void VplusScan(std::vector<uint8_t> pVplusValues);
	uint8_t FitVplusVcth(bool pDoDraw, uint8_t pTargetVcth);
    
    
private:
		
	TFile* fResultFile;
	bool fHole;

	TestGroupMap fTestGroupMap;

private:
    
    void Initialise();
	void ConstructTestGroup(uint8_t pBeId, uint8_t FeId, uint8_t CbcId);


};

#endif
