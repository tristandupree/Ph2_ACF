#include "HybridTester.h"

// fill the Histograms, count the hits and increment Vcth

struct CbcVisitor  : public HwDescriptionVisitor
{
	TH1F* fBotHist;
	TH1F* fTopHist;
	const Event* fEvent;

	CbcVisitor( TH1F* pBotHist, TH1F* pTopHist, const Event* pEvent ): fBotHist( pBotHist ), fTopHist( pTopHist ), fEvent( pEvent ) {}

	void visit( Cbc& pCbc ) {
		for ( uint32_t cId = 0; cId < NSENSOR; cId++ ) {
			if ( fEvent->DataBit( pCbc.getFeId(), pCbc.getCbcId(), cId ) ) {
				int globalChannel = 254 * pCbc.getCbcId() + cId;

				if ( globalChannel % 2 == 0 ) fBotHist->Fill( globalChannel );
				else fTopHist->Fill( globalChannel );
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


const std::string currentDateTime()
{
	time_t now = time( 0 );
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime( &now );
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime( buf, sizeof( buf ), "_%d-%m-%y_%H:%M", &tstruct );

	return buf;
}

void scanpause()
{
	std::cout << "Found a Threshold with 0 Hits - Start external Signal source and press [Enter] to continue ...";
	std::cin.get();
}


void HybridTester::InitializeHists()
{
	//  special Visito class to count objects
	Counter cCbcCounter;
	accept( cCbcCounter );
	fNCbc = cCbcCounter.getNCbc();

	fDataCanvas = new TCanvas( "Data", "Different hits counters", 1200, 800 );
	fDataCanvas->Divide( 2 );

	TString cFrontName( "fHistTop" );
	TObject* cObj = gROOT->FindObject( cFrontName );
	if ( cObj ) delete cObj;

	fHistTop = new TH1F( cFrontName, "Front Face; Channel Number; Number of Hits", ( fNCbc / 2 * 254 ) + 1, -0.5, ( fNCbc / 2 * 254 ) + .5 );
	fDataCanvas->cd( 1 );
	fHistTop->Draw();

	TString cBackName( "fHistBottom" );
	cObj = gROOT->FindObject( cBackName );
	if ( cObj ) delete cObj;

	fHistBottom = new TH1F( cFrontName, "Back Face; Channel Number; Number of Hits", ( fNCbc / 2 * 254 ) + 1, -0.5, ( fNCbc / 2 * 254 ) + .5 );
	fDataCanvas->cd( 2 );
	fHistBottom->Draw();

}

void HybridTester::CreateResultDirectory( std::string pDirname )
{

	bool cHoleMode = fSettingsMap.find( "HoleMode" )->second;

	std::string cMode;
	if ( cHoleMode ) cMode = "_Hole";
	else cMode = "_Electron";

	pDirname = pDirname + cMode +  currentDateTime();
	std::cout << "Creating directory: " << pDirname << std::endl;
	std::string cCommand = "mkdir -p " + pDirname;

	system( cCommand.c_str() );

	fDirName = pDirname;
}

void HybridTester::InitResultFile()
{

	if ( !fDirName.empty() )
	{
		std::string cFilename = fDirName + "/HybridTestResults.root";
		fResultFile = TFile::Open( cFilename.c_str(), "RECREATE" );
	}
	else std::cout << RED << "ERROR: " << RESET << "No Result Directory initialized - not saving results!" << std::endl;
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

void HybridTester::Measure()
{
	uint32_t cTotalEvents = fSettingsMap.find( "NEvents" )->second;

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

					if ( cN % 10 == 0 ) fDataCanvas->Update();

					cN++;

					if ( cN < cTotalEvents )
						cEvent = fBeBoardInterface->GetNextEvent( &pBoard );
					else break;
				}
				cNthAcq++;
			} // End of Analyze Events of last Acquistion loop
		}
	}
}

void HybridTester::SaveResults()
{
	fHistTop->Write( fHistTop->GetName(), TObject::kOverwrite );
	fHistBottom->Write( fHistBottom->GetName(), TObject::kOverwrite );
	fDataCanvas->Write( fDataCanvas->GetName(), TObject::kOverwrite );

	fResultFile->Write();
	fResultFile->Close();

	std::cout << "Resultfile written correctly!" << std::endl;
}

