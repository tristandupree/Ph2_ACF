#ifndef __ANALYSER_H__
#define __ANALYSER_H__
#include "Utilities.h"
#include "TH1F.h"
#include "../HWDescription/Cbc.h"
#include "Event.h"
#include <vector>
#include <set>
#include <TString.h>

using namespace Ph2_HwDescription;

class TPad;

namespace Ph2_HwInterface
{

	class HistGroup;
	typedef std::map<Int_t, HistGroup*> HistGroupMap;

	class Analyser {

		public:
			Analyser( uint32_t pBeId, uint32_t pNFe, uint32_t pNCbc,
					Bool_t pNegativeLogicCbc,
					const char *pOutputDir);
			virtual ~Analyser();
			virtual void Initialise();
			virtual uint32_t   Analyse(const Event *pEvent, bool pFillDataStream = true );
			virtual void DrawHists();
			virtual void SetHistPad( uint32_t pFeId, uint32_t pCbcId, TPad *pPad );
			void ShowDataStream( Bool_t pShow ) { fShowDataStream = pShow; }
			Bool_t ShowDataStream() { return fShowDataStream; }

		protected:
			virtual Int_t getHistGroupId( uint32_t pFeId, uint32_t pCbcId );

			uint32_t                fBeId;
			uint32_t                fNFe;
			uint32_t                fNCbc;
			bool                    fNegativeLogicCbc;
			TString                 fOutputDir;
			Bool_t                  fShowDataStream;
			HistGroupMap            fHistGroupMap;
	};

	class HistGroup
	{
		friend class Analyser;

		protected:
			std::pair<TH1F *, TPad *> fDataStream;

		public:
			HistGroup( uint32_t pBeId, uint32_t pFeId, uint32_t pCbcId );
			~HistGroup();
	};
}
#endif
