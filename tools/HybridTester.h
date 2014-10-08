
#ifndef HybridTester_h__
#define HybridTester_h__

#include "../HWDescription/Module.h"
#include "../HWDescription/Cbc.h"
#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../System/SystemController.h"
#include "../System/ConsoleColor.h"
#include "../HWInterface/Visitor.h"



#include "../System/SystemController.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;
using namespace Ph2_System;

class HybridTester : public SystemController
{
  public:
	HybridTester() {}
	~HybridTester() {}
	void InitializeHists();
	void CreateResultDirectory( std::string pDirectoryname );
	void InitResultFile();
	void ScanThreshold();
	void Measure();
	void SaveResults();

  private:
	uint32_t fNCbc;

	std::string fDirName;
	TFile* fResultFile;
	TCanvas* fDataCanvas;
	TH1F* fHistTop;
	TH1F* fHistBottom;
};


#endif


