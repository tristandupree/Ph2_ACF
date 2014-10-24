/*!

        \file                   HybridTester.h
        \brief                 class for validating x*CBC2 hybrids
        \author              Alireza KOKABI & Georg AUZINGER
        \version                1.0
        \date                   24/10/14
        Support :               mail to : georg.auzinger@cern.ch

 */

#ifndef HybridTester_h__
#define HybridTester_h__

#include "../HWDescription/Module.h"
#include "../HWDescription/Cbc.h"
#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../System/SystemController.h"
#include "../Utils/ConsoleColor.h"
#include "../Utils/Visitor.h"
#include "../Utils/Utilities.h"
#include "../Utils/CommonVisitors.h"



#include "../System/SystemController.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TStyle.h"
#include "TLine.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;
using namespace Ph2_System;

/*!
 * \class HybridTester
 * \brief Class to test x*CBC2 Hybrids
 */
class HybridTester : public SystemController
{
  public:
	// Default C'tor
	HybridTester() {}
	// Default D'tor
	~HybridTester() {}
	/*!
	* \brief Initialize the Histograms and Canvasses for CMD line applications
	* \param pThresholdScan :  bool flag to initialize the additional canvas for the Threshold scan
	*/
	void Initialize( bool pThresholdScan );
	/*!
	* \brief Initialize the Histograms and Canvasses for GUI applications
	* \param pThresholdScan :  bool flag to initialize the additional canvas for the Threshold scan
	* \param pCanvasVector: vector of TCanvas* to be passed by the GUI to draw on
	*/
	void InitializeGUI( bool pThresholdScan, std::vector<TCanvas*> pCanvasVector );
	/*!
	* \brief Test CBC registers by writing complimentary bit patterns (0x55, 0xAA)
	*/
	void TestRegisters();
	/*!
	* \brief Scan the thresholds to identify the threshold with no noise occupancy
	*/
	void ScanThreshold();
	/*!
	* \brief Measure the single strip efficiency
	*/
	void Measure();
	/*!
	* \brief Save the results to the file created with SystemController::InitializeResultFile
	*/
	void SaveResults();

  private:
	uint32_t fNCbc;   /*!< Number of CBCs in the Setup */
	TCanvas* fDataCanvas;   /*!<Canvas to output single-strip efficiency */
	TH1F* fHistTop;   /*!< Histogram for top pads */
	TH1F* fHistBottom;   /*!< Histogram for bottom pads */

	TCanvas* fSCurveCanvas;   /*!< Canvas for threshold scan */
	TH1F* fSCurve;   /*!< Histogram for SCurve */
	TF1* fFit;   /*!< fit for SCurve*/

	/*!
	* \brief private method that calls the constructors for the histograms
	*/
	void InitializeHists();
	/*!
	* \brief private method to periodically update the output graphs
	*/
	void UpdateHists() {
		fDataCanvas->cd( 1 );
		fHistTop->Draw();
		fDataCanvas->cd( 2 );
		fHistBottom->Draw();
		fDataCanvas->Update();
	}

};


#endif


