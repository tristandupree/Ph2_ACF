#ifndef _CHANNEL_H__
#define _CHANNEL_H__

#include <map>
#include <vector>
#include "TH1F.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TString.h"
#include "TROOT.h"

struct Channel{

	Channel(uint8_t pBeId, uint8_t pFeId, uint8_t pCbcId, uint8_t pChannelId);
	~Channel();

	// members
	// Ids
	uint8_t fBeId;
	uint8_t fFeId;
	uint8_t fCbcId;
	uint8_t fChannelId;

	// Settings
	uint8_t fOffset;

	// Data
	TH1F* fScurve;
	TF1*  fFit;

	// Methods
	double getPedestal();
	double getNoise();
	uint8_t getOffset( return fOffset );
	void setOffset(uint8_t pOffset);

	void initializeHist(uint8_t pValue, bool pVplusScan);
	void fillHist(uint8_t pVcth);
	void fitHist(uint8_t pEventsperVcth, bool pHole, uint8_t pVplus, TFile* pResultfile);
	void resetHist();
};

struct TestGroup{
	TestGroup(uint8_t pBeId,uint8_t pFeId,uint8_t pCbcId,uint8_t pGroupId);

	void FillVplusVcthGraph(uint8_t pVplus, double pPedestal, double pNoise);
	uint8_t fBeId;
	uint8_t fFeId;
	uint8_t fCbcId;
	uint8_t fGroupId;
	TGraphErrors* fVplusVcthGraph;
};

struct TestGroupComparer{
	bool operator() (const TestGroup &g1, const TestGroup &g2) const {
		if (g1.BeId == g2.BeId) return g1.FeId < g2.FeId;
		else if (g1.FeId == g2.FeId) return g1.CbcId < g2.CbcId;
		else if (g1.CbcId == g2.CbcId) return g1.GroupId < g2.GroupId;
		else return g1.BeId < g2.BeId;
	}
};

typedef std::map< TestGroup , std::vector<Channel>, TestGroupComparer > TestGroupMap;

#endif
