#include "HybridTester.h"

// fill the Histograms, count the hits and increment Vcth

struct CbcVisitor  : public HwDescriptionVisitor
{
	TH1F* fBotHist;
	TH1F* fTopHist;
	const Event* fEvent;

	CbcVisitor( TH1F* pBotHist, TH1F* pTopHist, const Event* pEvent ): fBotHist( pBotHist ), fTopHist( pTopHist ), fEvent( pEvent ) {}

	void visit( Cbc& pCbc ) {
		std::vector<bool> cDataBitVector = fEvent->DataBitVector( pCbc.getFeId(), pCbc.getCbcId() );
		for ( uint32_t cId = 0; cId < NSENSOR; cId++ ) {
			if ( cDataBitVector.at( cId ) ) {
				uint32_t globalChannel = ( pCbc.getCbcId() * 254 ) + cId;

				// find out why histograms are not filling!
				if ( globalChannel % 2 == 0 )
					fBotHist->Fill( globalChannel / 2 );
				else
					fTopHist->Fill( ( globalChannel - 1 ) / 2 );

			}
		}
	}
};


struct CbcHitCounter  : public HwDescriptionVisitor
{
	const Event* fEvent;
	uint32_t fHitcounter = 0;
	CbcHitCounter( const Event* pEvent ): fEvent( pEvent ) {}

	void visit( Cbc& pCbc ) {
		for ( uint32_t cId = 0; cId < NSENSOR; cId++ ) {
			if ( fEvent->DataBit( pCbc.getFeId(), pCbc.getCbcId(), cId ) ) fHitcounter++;
		}
	}
};

struct CbcRegModifier : public HwDescriptionVisitor
{
	CbcInterface* fInterface;
	uint8_t fVcth;
	bool fHoleMode;

	CbcRegModifier( CbcInterface* pInterface, bool pHoleMode ): fInterface( pInterface ), fHoleMode( pHoleMode ) {}

	void visit( Cbc& pCbc ) {
		fVcth = pCbc.getReg( "VCth" );
		int increment;
		if ( fHoleMode ) fInterface->WriteCbcReg( &pCbc, "VCth", fVcth += 1 );
		else fInterface->WriteCbcReg( &pCbc, "VCth", fVcth -= 1 );
	}
};


void scanpause()
{
	std::cout << "Found a Threshold with 0 Hits - Start external Signal source and press [Enter] to continue ...";
	std::cin.get();
}


void HybridTester::InitializeHists()
{
	gStyle->SetOptStat( 000000 );
	gStyle->SetTitleOffset( 1.3, "Y" );
	//  special Visito class to count objects
	Counter cCbcCounter;
	accept( cCbcCounter );
	fNCbc = cCbcCounter.getNCbc();

	fDataCanvas = new TCanvas( "Data", "Different hits counters", 1200, 800 );
	fDataCanvas->Divide( 2 );

	TString cFrontName( "fHistTop" );
	fHistTop = ( TH1F* )( gROOT->FindObject( cFrontName ) );
	if ( fHistTop ) delete fHistTop;

	fHistTop = new TH1F( cFrontName, "Front Pad Channels; Pad Number; Occupancy [%]", ( fNCbc / 2 * 254 ) , -0.5, ( fNCbc / 2 * 254 ) + .5 );

	TString cBackName( "fHistBottom" );
	fHistBottom = ( TH1F* )( gROOT->FindObject( cBackName ) );
	if ( fHistBottom ) delete fHistBottom;

	fHistBottom = new TH1F( cBackName, "Back Pad Channels; Pad Number; Occupancy [%]", ( fNCbc / 2 * 254 ) , -0.5, ( fNCbc / 2 * 254 ) + .5 );


}



void HybridTester::ScanThreshold()
{

	uint32_t cEventsperVcth = 10;
	bool cHoleMode = fSettingsMap.find( "HoleMode" )->second;
	uint32_t cHitCounter = 1;

	// stop increasing the threshold when the occupancy has reached 1%
	// re-think this! TODO
	while ( cHitCounter > 0.01 * fNCbc * NSENSOR )
	{
		for ( auto& cShelve : fShelveVector )
		{
			for ( BeBoard& pBoard : cShelve->fBoardVector )
			{
				uint32_t cN = 0;
				uint32_t cNthAcq = 0;
				cHitCounter = 0;

				while ( cN <  cEventsperVcth )
				{
					Run( &pBoard, cNthAcq );

					const Event* cEvent = fBeBoardInterface->GetNextEvent( &pBoard );

					// Loop over Events from this Acquisition
					while ( cEvent )
					{
						if ( cN == cEventsperVcth )
							break;

						CbcHitCounter cHitcounter( cEvent );
						pBoard.accept( cHitcounter );
						cHitCounter += cHitcounter.fHitcounter;
						cN++;

						if ( cN < cEventsperVcth )
							cEvent = fBeBoardInterface->GetNextEvent( &pBoard );
						else break;
					}
					cNthAcq++;
				}
				CbcRegModifier cModifier( fCbcInterface, cHoleMode );
				pBoard.accept( cModifier );
				std::cout << "Counted " << cHitCounter << " Hits on Hybird for Vcth " << int( cModifier.fVcth ) << " - increasing Vcth by 1 unit!" << std::endl;
			}
		}
	}
	scanpause();
}

void HybridTester::TestRegisters()
{
	struct RegTester : public HwDescriptionVisitor
	{
		CbcInterface* fInterface;
		std::string<std::string> fBadRegisters;
		RegTester( CbcInterface* pInterface ): fInterface( pInterface ) {}

		void visit( Cbc& pCbc ) {
			uint8_t cFirstBitPattern = 0xAA;
			uint8_t cSecondBitPattern = 0x55;

			CbcRegMap cMap = pCbc.getRegMap();
			for ( const auto& cReg : cMap ) {
				fInterface->WriteCbcReg( &pCbc, cReg.first, cFirstBitPattern, true );
				fInterface->WriteCbcReg( &pCbc, cReg.first, cSecondBitPattern, true );

			}
		};
	};

	RegTester cRegTester( fCbcInterface );
	accept( cRegTester );
}

void HybridTester::Measure()
{
	uint32_t cTotalEvents = fSettingsMap.find( "Nevents" )->second;

	for ( auto& cShelve : fShelveVector )
	{
		for ( BeBoard& pBoard : cShelve->fBoardVector )
		{
			uint32_t cN = 0;
			uint32_t cNthAcq = 0;

			while ( cN <  cTotalEvents )
			{
				Run( &pBoard, cNthAcq );

				const Event* cEvent = fBeBoardInterface->GetNextEvent( &pBoard );
				// Loop over Events from this Acquisition
				while ( cEvent )
				{
					if ( cN == cTotalEvents )
						break;

					CbcVisitor cFiller( fHistBottom, fHistTop, cEvent );
					pBoard.accept( cFiller );

					if ( cN % 100 == 0 )
						UpdateHists();

					cN++;

					if ( cN < cTotalEvents )
						cEvent = fBeBoardInterface->GetNextEvent( &pBoard );
					else break;
				}
				cNthAcq++;
			} // End of Analyze Events of last Acquistion loop
		}
	}
	fHistTop->Scale( 100 / double_t( cTotalEvents ) );
	fHistTop->GetYaxis()->SetRangeUser( 0, 100 );
	fHistBottom->Scale( 100 / double_t( cTotalEvents ) );
	fHistBottom->GetYaxis()->SetRangeUser( 0, 100 );
	UpdateHists();
}

void HybridTester::SaveResults()
{
	fHistTop->Write( fHistTop->GetName(), TObject::kOverwrite );
	fHistBottom->Write( fHistBottom->GetName(), TObject::kOverwrite );
	fDataCanvas->Write( fDataCanvas->GetName(), TObject::kOverwrite );

	fResultFile->Write();
	fResultFile->Close();


	std::cout << "Resultfile written correctly!" << std::endl;

	std::string cPdfName = fDirectoryName + "/HybridTestResults.pdf";
	fDataCanvas->SaveAs( cPdfName.c_str() );

}


