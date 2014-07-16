#include "Analyser.h"
#include "TString.h"
#include "TCanvas.h"
#include <TFile.h>
#include <TLegend.h>
#include <TThread.h>
#include <TROOT.h>
#include "TGFrame.h"
#include "../HWDescription/Cbc.h"

namespace Ph2_HwInterface
{

	//--------------------------------------------------------------------------
	//Analyser Class

	Analyser::Analyser( uint32_t pBeId, uint32_t pNFe, uint32_t pNCbc,
			Bool_t pNegativeLogicCbc,
			const char *pOutputDir):
		fBeId(pBeId), fNFe(pNFe), fNCbc(pNCbc),
		fNegativeLogicCbc(pNegativeLogicCbc),
		fOutputDir( pOutputDir ),
		fShowDataStream( kFALSE )

    {

	}


	Analyser::~Analyser()
    {
		for( HistGroupMap::iterator cIt = fHistGroupMap.begin(); cIt != fHistGroupMap.end(); cIt++ )
        {
			delete cIt->second;
		}
	}


	void Analyser::Initialise()
	{
		for( uint32_t cFeId = 0; cFeId < fNFe; cFeId++ )
		{
			for( uint32_t cCbcId=0; cCbcId < fNCbc; cCbcId++ )
			{
				Int_t cHistGroupId = getHistGroupId( cFeId, cCbcId );
				HistGroupMap::iterator cIt = fHistGroupMap.find( cHistGroupId );

				if( cIt != fHistGroupMap.end() )
				{
					delete cIt->second;
					fHistGroupMap.erase(cIt);
				}

				HistGroup * cHistGroup = new HistGroup( fBeId, cFeId, cCbcId );
				fHistGroupMap.insert( std::pair<uint32_t, HistGroup *>( cHistGroupId, cHistGroup ) );
			}
		}
#ifdef __CBCDAQ_DEV__
			std::cout << "Analyser::Initialise() done" << std::endl;
#endif
	}


	uint32_t Analyser::Analyse( const Event *pEvent, bool pFillDataStream )
	{
		if( !pFillDataStream || !fShowDataStream ) return 0;

		int cHits(0);
		for( uint32_t cFeId = 0; cFeId < fNFe; cFeId++ )
		{
			const FeEvent *cFeEvent = pEvent->GetFeEvent( cFeId );

			if( cFeEvent == 0 )
				continue;

			for( uint32_t cCbcId=0; cCbcId < fNCbc; cCbcId++ )
			{
				const CbcEvent *cCbcEvent = cFeEvent->GetCbcEvent( cCbcId );

				if( cCbcEvent == 0 )
					continue;

				Int_t cId = getHistGroupId( cFeId, cCbcId );
				HistGroupMap::iterator cIt = fHistGroupMap.find( cId );

				if( cIt == fHistGroupMap.end() )
					continue;

				HistGroup *cHistGroup = cIt->second;
				cHistGroup->fDataStream.first->Reset();

				for( uint32_t cId = 0; cId < CbcEvent::NSENSOR; cId++ )
				{
					bool cBitValue = cCbcEvent->DataBit( cId );

					if( cBitValue )
					{
						cHits++;
						cHistGroup->fDataStream.first->Fill( cId );
					}
				}
			}
		}

		return cHits;
	}


	void Analyser::SetHistPad( uint32_t pFeId, uint32_t pCbcId, TPad *pPad )
	{
		Int_t cHistGroupId = getHistGroupId( pFeId, pCbcId );

		HistGroupMap::iterator cIt = fHistGroupMap.find( cHistGroupId );
		HistGroup *cGroup = cIt->second;
		cGroup->fDataStream.second = pPad;
	}


	Int_t Analyser::getHistGroupId( uint32_t pFeId, uint32_t pCbcId )
	{
		return fNCbc * pFeId + pCbcId;
	}


	void Analyser::DrawHists()
	{
		if( !( fShowDataStream ) )
			return;

		TPad *cPad(0);

		HistGroupMap::iterator cIt = fHistGroupMap.begin();

		for( ; cIt != fHistGroupMap.end(); cIt++ )
		{
			std::pair<TH1F *, TPad *> cDataStream = cIt->second->fDataStream;
			cPad = (TPad *)cDataStream.second->cd();
			cDataStream.first->Draw();
			cPad->Update();
		}
	}

	//--------------------------------------------------------------------------
	//HistGroup Class

	HistGroup::HistGroup( uint32_t pBeId, uint32_t pFeId, uint32_t pCbcId )
	{
		TString cHname = Form( "hDataStreamBE%uFE%uCBC%u", pBeId, pFeId, pCbcId );
		TObject *cObj = gROOT->FindObject( cHname );

		if( cObj )
			delete cObj;

		TH1F *cHist = new TH1F( cHname,
				Form( "Data stream BE: %u FE:%u CBC:%u", pBeId, pFeId, pCbcId ),
				CbcEvent::NSENSOR, -0.5, CbcEvent::NSENSOR-0.5 );

		fDataStream = std::pair<TH1F *, TPad *>( cHist, 0 );
	}


	HistGroup::~HistGroup()
	{
		//delete fDataStream.first;
	}
}
