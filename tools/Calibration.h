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

#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TFitResultPtr.h"

#include <map>

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
	Calibration(std::string pResultfilepath);

	// D'Tor
	~Calibration();

	void InitialiseTestGroup();

	void VplusScan();
	void FitVplusVcth(bool pDoDraw, uint8_t pTargetVcth);
    void setGlobalReg(uint8_t pShelveId, uint8_t pBeId, std::string pRegName, uint8_t pRegValue);
    void EnableTestGroup(uint8_t pShelveId, uint8_t pBeId, uint8_t pGroupId, uint8_t pVplus);
    void FillScurveHists(uint8_t pShelveId, uint8_t pBeId, uint8_t pGroupId, uint8_t pVcth, const Event* pEvent);
    void processSCurves(uint8_t pShelveId, uint8_t pBeId, uint8_t pGroupId, uint32_t pEventsperVcth, uint8_t pVplus, bool pHoleMode);
    
private:
		
	TFile* fResultFile;
	TestGroupMap fTestGroupMap;
	TestGroupGraphMap fTestGroupGraphMap;
	std::vector<uint8_t> fVplusValues;

private:
    
	void ConstructTestGroup(uint8_t pShelveId, uint8_t pBeId, uint8_t pFeId, uint8_t pCbcId);

};

#endif
