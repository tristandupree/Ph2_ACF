#include "Channel.h"

Double_t MyErf( Double_t *x, Double_t *par ){
	Double_t x0 = par[0];
	Double_t width = par[1];
	Double_t fitval(0);
	if( x[0] < x0 ) fitval = 0.5 * TMath::Erfc( ( x0 - x[0] )/width );
	else fitval = 0.5 + 0.5 * TMath::Erf( ( x[0] - x0 )/width );
	return fitval;
}

Channel::Channel(uint8_t pBeId, uint8_t pFeId, uint8_t pCbcId, uint8_t pChannelId) :
fBeId( pBeId ),
fFeId( pFeId ),
fCbcId( pCbcId ),
fChannelId( pChannelId ){

	TString histname = Form("Scurve_Be%d_Fe%d_Cbc%d_Channel%d", fBeId, fFeId, fCbcId, fChannelId);
	TString fitname = Form("Fit_Be%d_Fe%d_Cbc%d_Channel%d", fBeId, fFeId, fCbcId, fChannelId);

	TH1F* fScurve = (TH1F*) gROOT->FindObject(histname);
	if( fScurve ) delete fScurve;
	else fScurve = new TH1F(histname,Form("Scurve_Be%d_Fe%d_Cbc%d_Channel%d", fBeId, fFeId, fCbcId, fChannelId),256, -0.5, 255.5);

	TF1* fFit = (TF1*) gROOT->FindObject(fitname);
	if( fFit ) delete fFit;
	fFit = NULL;
}

Channel::~Channel(){}

uint8_t Channel::getPedestal(){

	if( fFit !=NULL ){
		return fabs(fFit->GetParameter(0));
	}
	else return -1;
}

uint8_t Channel::getNoise(){

	if( fFit !=NULL ){
		return fabs(fFit->GetParameter(1));
	}
	else return -1;
}

void Channel::setOffset( uint8_t pOffset ){
	fOffset = pOffset;
}

void fillHist(uint8_t pVcth, bool pHit){
	if( pHit ){
		fScurve->Fill(pVcth);
	}
}

void fitHist(uint8_t pEventsperVcth, bool pHole, TFile* pResultfile){

	if ( fFit == NULL ){

		// Normalize first
		fScurve->Sumw2();
		fScurve->Scale(h, 1/pEventsperVcth);

		// Get first non 0 and first 1
		double cFirstNon0(0);
		double cFirst1(0); 

		// Not Hole Mode
		if( !pHole ){
			for( Int_t cBin = 1; cBin <= fScurve->GetNbinsX(); cBin++ ){
				double cContent = fScurve->GetBinContent( cBin );
				if( !cFirstNon0	){
					if( cContent ) cFirstNon0 = fScurve->GetBinCenter(cBin);
				}
				else if( cContent == 1 ) {
					cFirst1 = fScurve->GetBinCenter(cBin); 
					break;
				}
			}
		}
		// Hole mode
		else{
			for( Int_t cBin = fScurve->GetNbinsX(); cBin >=1; cBin-- ){
				double cContent = fScurve->GetBinContent( cBin );
				if( !cFirstNon0	){
					if( cContent ) cFirstNon0 = fScurve->GetBinCenter(cBin);
				}
				else if( cContent == 1 ) {
					cFirst1 = fScurve->GetBinCenter(cBin); 
					break;
				}
			}
		}

		// Get rough midpoint & width
		double cMid = ( cFirst1 + cFirstNon0 ) * 0.5;
		double cWidth = ( cFirst1 - cFirstNon0 ) * 0.5;

		// create fit
		TString fitname = Form("Fit_Be%d_Fe%d_Cbc%d_Channel%d", fBeId, fFeId, fCbcId, fChannelId);
		fFit = new TF1(fitname, MyErf, pMin, pMax, 2 );
		fFit->SetParameters(cMid, cWidth);

		// Fit
		fScurve->Fit(fitname,"RSLQ");

		// Eventually add TFitResultPointer
		// create a Directory in the file for the current Offset and save the channel Data
		TString cDirName = Form("Offset%d",fOffset);
		TObject* cObj = gROOT->FindObject(cDirName);
		if (!cObj) pResultfile->mkdir(cDirName);
		pResultfile->cd(cDirName);

		fScurve->Write(fScurve->GetName(), TObject::kOverwrite);
		fFit->Write(fFit->GetName(), TObject::kOverwrite);

		pResultfile->cd();
	}
	else std::cout << "Historgram Empty for Fe " << fFeId << " Cbc " << fCbcId << " Channel " << fChannelId << std::endl;


}