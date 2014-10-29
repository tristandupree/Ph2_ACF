#include "datatestworker.h"
#include <QDebug>
#include <QThread>
#include <vector>

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
#include "TH1D.h"

#include "utils.h"
#include "unistd.h"


namespace GUI
{
    struct HistogramFiller  : public HwDescriptionVisitor
    {

        std::shared_ptr<TH1D>  fBotHist;
        std::shared_ptr<TH1D>  fTopHist;

        const Event* fEvent;

        HistogramFiller( std::shared_ptr<TH1D> pBotHist, std::shared_ptr<TH1D> pTopHist, const Event* pEvent ): fBotHist( pBotHist ), fTopHist( pTopHist ), fEvent( pEvent ) {}

        void visit( Cbc& pCbc ) {

            std::vector<bool> cDataBitVector = fEvent->DataBitVector( pCbc.getFeId(), pCbc.getCbcId() );
            for ( uint32_t cId = 0; cId < NCHANNELS; cId++ ) {
                if ( cDataBitVector.at( cId ) ) {
                    uint32_t globalChannel = ( pCbc.getCbcId() * 254 ) + cId;
                                  std::cout << "Channel " << globalChannel << " VCth " << ( int )pCbc.getReg( "VCth" ) << std::endl;
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
            for ( uint32_t cId = 0; cId < NCHANNELS; cId++ ) {
                if ( fEvent->DataBit( pCbc.getFeId(), pCbc.getCbcId(), cId ) ) fHitcounter++;
            }
        }
    };

    DataTestWorker::DataTestWorker(QObject *parent,
                                   SystemController &sysController) ://, SystemController &sysCtrl) :
        QObject(parent),
        m_systemController(sysController)
    {
    }

    DataTestWorker::~DataTestWorker()
    {
        qDebug() << "Destructing " << this;
    }

    void DataTestWorker::requestWork(int cVcth, int cEvents)
    {
        m_Vcth = cVcth;
        m_Events = cEvents;

        qDebug()<<"Request worker start in Thread "<<thread()->currentThreadId();

        emit workRequested();
    }

    void DataTestWorker::abort()
    {
        //TODO
        /*mutex.lock();
        if (_working) {
            _abort = true;
            qDebug()<<"Request worker aborting in Thread "<<thread()->currentThreadId();
        }
        mutex.unlock();*/
    }

    void DataTestWorker::doWork()
    {
        std::string cHWFile = ( "settings/Calibration2CBC.xml");
        std::string cDirectory = ("Results/");
        bool cScan=false;

        fBeBoardInterface = m_systemController.getBeBoardInterface();
        fCbcInterface = m_systemController.getCbcInterface();
        fShelveVector = m_systemController.getfShelveVector();
        fBeBoardFWMap = m_systemController.getBeBoardFWMap();

        Initialise(cScan);
        //m_systemController.m_worker->ConfigureHw();
        //TestRegisters();

        Measure();


        emit finished();

    }

    void DataTestWorker::Initialise(bool pThresholdScan)
    {
        gStyle->SetOptStat( 000000 );
        gStyle->SetTitleOffset( 1.3, "Y" );
        //  special Visito class to count objects
        Counter cCbcCounter;
        m_systemController.m_worker->accept(cCbcCounter);
        fNCbc = cCbcCounter.getNCbc();

        InitialiseHists();

    }

    void DataTestWorker::InitialiseHists()
    {
        fNCbc = 2;

        TString cFrontName( "fHistTop" );
        /*fHistTop = ( TH1F* )( gROOT->FindObject( cFrontName ) );
        if ( fHistTop ) delete fHistTop;

        fHistTop = new TH1F( cFrontName, "Front Pad Channels; Pad Number; Occupancy [%]", ( fNCbc / 2 * 254 ) + 1, -0.5, ( fNCbc / 2 * 254 ) + .5 );
        fHistTop->SetFillColor( 4 );
        fHistTop->SetFillStyle( 3001 );*/

        auto h1 = std::make_shared<TH1D>(cFrontName, "Front Pad Channels; Pad Number; Occupancy [%]", ( fNCbc / 2 * 254 ) + 1, -0.5, ( fNCbc / 2 * 254 ) + .5);
        m_vecHist.push_back(h1);
        m_vecHist.at(0)->SetFillColor(4);
        m_vecHist.at(0)->SetFillStyle(3001);

        TString cBackName( "fHistBottom" );
        /*fHistBottom = ( TH1F* )( gROOT->FindObject( cBackName ) );
        if ( fHistBottom ) delete fHistBottom;

        fHistBottom = new TH1F( cBackName, "Back Pad Channels; Pad Number; Occupancy [%]", ( fNCbc / 2 * 254 ) + 1 , -0.5, ( fNCbc / 2 * 254 ) + .5 );
        fHistBottom->SetFillColor( 4 );
        fHistBottom->SetFillStyle( 3001 );*/

        auto h2 = std::make_shared<TH1D>( cBackName, "Back Pad Channels; Pad Number; Occupancy [%]", ( fNCbc / 2 * 254 ) + 1 , -0.5, ( fNCbc / 2 * 254 ) + .5 );
        m_vecHist.push_back(h2);

        emit sendGraphData(m_vecHist);

    }

    void DataTestWorker::ScanThreshold()
    {
        std::cout << "Scanning noise Occupancy to find threshold for test with external source ... " << std::endl;

        //auto cSetting = fSettingsMap.find( "HoleMode" );
        //bool cHoleMode = ( cSetting != std::end( fSettingsMap ) ) ? cSetting->second : true;
        bool cHoleMode = true;

        // Necessary variables
        uint32_t cEventsperVcth = 10;
        bool cNonZero = false;
        bool cAllOne = false;
        uint32_t cAllOneCounter = 0;
        uint8_t cVcth, cDoubleVcth;
        ( cHoleMode ) ? cVcth = 0xFF : cVcth = 0x00;
        int cStep = ( cHoleMode ) ? -10 : 10;

        // Root objects
        fSCurve = new TH1F( "fSCurve", "Noise Occupancy; VCth; Counts", 255, 0, 255 );
        fSCurve->SetMarkerStyle( 8 );
        //fSCurveCanvas->cd();
        fFit = new TF1( "fFit", MyErf, 0, 255, 2 );

        // Adaptive VCth loop
        while ( 0x00 <= cVcth && cVcth <= 0xFF )
        {
            if ( cAllOne ) break;
            if ( cVcth == cDoubleVcth )
            {
                cVcth +=  cStep;
                continue;
            }

            // Set current Vcth value on all Cbc's
            CbcRegWriter cWriter( fCbcInterface, "VCth", cVcth );
            m_systemController.m_worker->accept( cWriter );

            uint32_t cN = 0;
            uint32_t cNthAcq = 0;
            uint32_t cHitCounter = 0;

            // maybe restrict to pBoard? instead of looping?
            for ( auto& cShelve : fShelveVector )
            {
                if ( cAllOne ) break;
                for ( BeBoard& pBoard : cShelve->fBoardVector )
                {
                    while ( cN <  cEventsperVcth )
                    {
                        m_systemController.m_worker->Run( &pBoard, cNthAcq );

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
                    } // done with this acquisition

                    fSCurve->SetBinContent( cVcth, cHitCounter );
                    fSCurve->Draw( "P" );
                    fSCurveCanvas->Update();
                    // check if the hitcounter is all ones

                    if ( cNonZero == false && cHitCounter != 0 )
                    {
                        cDoubleVcth = cVcth;
                        cNonZero = true;
                        cVcth -= 2 * cStep;
                        cStep /= 10;
                        continue;
                    }
                    if ( cHitCounter > 0.95 * cEventsperVcth * fNCbc * NCHANNELS ) cAllOneCounter++;
                    if ( cAllOneCounter >= 10 ) cAllOne = true;
                    if ( cAllOne ) break;
                    cVcth += cStep;
                }
            }
        } // end of VCth loop

        // Fit & Plot
        fSCurve->Scale( 1 / double_t( cEventsperVcth * fNCbc * NCHANNELS ) );
        // fSCurveCanvas->cd();
        //fSCurve->Draw( "P" );

        double cFirstNon0( 0 );
        double cFirst1( 0 );

        // Not Hole Mode
        if ( !cHoleMode )
        {
            for ( Int_t cBin = 1; cBin <= fSCurve->GetNbinsX(); cBin++ )
            {
                double cContent = fSCurve->GetBinContent( cBin );
                if ( !cFirstNon0 )
                {
                    if ( cContent ) cFirstNon0 = fSCurve->GetBinCenter( cBin );
                }
                else if ( cContent == 1 )
                {
                    cFirst1 = fSCurve->GetBinCenter( cBin );
                    break;
                }
            }
        }
        // Hole mode
        else
        {
            for ( Int_t cBin = fSCurve->GetNbinsX(); cBin >= 1; cBin-- )
            {
                double cContent = fSCurve->GetBinContent( cBin );
                if ( !cFirstNon0 )
                {
                    if ( cContent ) cFirstNon0 = fSCurve->GetBinCenter( cBin );
                }
                else if ( cContent == 1 )
                {
                    cFirst1 = fSCurve->GetBinCenter( cBin );
                    break;
                }
            }
        }

        // Get rough midpoint & width
        double cMid = ( cFirst1 + cFirstNon0 ) * 0.5;
        double cWidth = ( cFirst1 - cFirstNon0 ) * 0.5;


        fFit->SetParameter( 0, cMid );
        fFit->SetParameter( 1, cWidth );

        fSCurve->Fit( fFit, "RNQ+" );
       // fFit->Draw( "same" );

        // Save
        fSCurve->Write( fSCurve->GetName(), TObject::kOverwrite );
        fFit->Write( fFit->GetName(), TObject::kOverwrite );
        //fSCurveCanvas->Write( fSCurveCanvas->GetName(), TObject::kOverwrite );
        //std::string cPdfName = fDirectoryName + "/NoiseOccupancy.pdf";
        std::string cPdFName = "Results/NoiseOccupancy.pdf";
        //fSCurveCanvas->SaveAs( cPdfName.c_str() );

        // Set new VCth
        double_t pedestal = fFit->GetParameter( 0 );
        double_t noise = fFit->GetParameter( 1 );

        //cSetting = fSettingsMap.find( "Threshold_NSigmas" );
        //int cSigmas = ( cSetting != std::end( fSettingsMap ) ) ? cSetting->second : 4;
        int cSigmas = 4;

        uint8_t cThreshold = ceil( pedestal + cSigmas * fabs( noise ) );

        std::cout << "Identified a noise Occupancy of 50% at VCth " << int( pedestal ) << " -- increasing by " << cSigmas <<  " sigmas (" << fabs( noise ) << ") to " << int( cThreshold ) << " for Hybrid test!" << std::endl;

        TLine* cLine = new TLine( cThreshold, 0, cThreshold, 1 );
        cLine->SetLineWidth( 3 );
        cLine->SetLineColor( 2 );
        //cLine->Draw( "same" );
        //fSCurveCanvas->Update();

        CbcRegWriter cWriter( fCbcInterface, "VCth", cThreshold );
        m_systemController.m_worker->accept( cWriter );

        // Wait for user to acknowledge and turn on external Source!
        std::cout << "Identified the threshold for 0 noise occupancy - Start external Signal source!" << std::endl;
        mypause();
    }

    void DataTestWorker::TestRegisters()
    {
        // This method has to be followed by a configure call, otherwise the CBCs will be in an undefined state
        struct RegTester : public HwDescriptionVisitor
        {
            CbcInterface* fInterface;
            std::map<uint32_t, std::set<std::string>> fBadRegisters;
            RegTester( CbcInterface* pInterface ): fInterface( pInterface ) {
                std::set<std::string> tempset;
                fBadRegisters[0] = tempset;
                fBadRegisters[1] = tempset;
            }

            void visit( Cbc& pCbc ) {
                uint8_t cFirstBitPattern = 0xAA;
                uint8_t cSecondBitPattern = 0x55;

                CbcRegMap cMap = pCbc.getRegMap();
                for ( const auto& cReg : cMap ) {
                    if ( !fInterface->WriteCbcReg( &pCbc, cReg.first, cFirstBitPattern, true ) ) fBadRegisters[pCbc.getCbcId()] .insert( cReg.first );
                    if ( !fInterface->WriteCbcReg( &pCbc, cReg.first, cSecondBitPattern, true ) ) fBadRegisters[pCbc.getCbcId()] .insert( cReg.first );
                }
            }

            void dumpResult() {
                for ( const auto& cCbc : fBadRegisters ) {
                    std::cout << "Bad Registers on Cbc " << cCbc.first << " : " << std::endl;
                    for ( const auto& cReg : cCbc.second ) std::cout << cReg << std::endl;
                }
            }
        };

        // This should probably be done in the top level application but there I do not have access to the settings map

        std::cout << "Testing Cbc Registers one-by-one with complimentary bit-patterns (0xAA, 0x55) ..." << std::endl;
        RegTester cRegTester( fCbcInterface );
        m_systemController.m_worker->accept(cRegTester);
        cRegTester.dumpResult();
        std::cout << "Done testing registers, re-configuring to calibrated state!" << std::endl;
        m_systemController.m_worker->ConfigureHw(); //TODO should emit this as signal
    }

    void DataTestWorker::Measure()
    {
        std::cout << "Mesuring Efficiency per Strip ... " << std::endl;
        uint32_t cTotalEvents = 200; //TODO Add dial
        //auto cSetting = fSettingsMap.find( "Nevents" );
        //uint32_t cTotalEvents = ( cSetting != std::end( fSettingsMap ) ) ? cSetting->second : 200;
        std::cout << "Taking data with " << cTotalEvents << " Events!" << std::endl;

        CbcRegReader cReader( fCbcInterface, "VCth" );
        m_systemController.m_worker->accept(cReader); //TODO pass safe

        for ( auto& cShelve : fShelveVector )
        {
            for ( BeBoard& pBoard : cShelve->fBoardVector )
            {
                uint32_t cN = 0;
                uint32_t cNthAcq = 0;

                while ( cN <  cTotalEvents )
                {
                    m_systemController.m_worker->Run( &pBoard, cNthAcq );

                    const Event* cEvent = fBeBoardInterface->GetNextEvent( &pBoard );
                    // Loop over Events from this Acquisition
                    while ( cEvent )
                    {

                        if ( cN == cTotalEvents )
                            break;

                        //HistogramFiller cFiller( fHistBottom, fHistTop, cEvent );
                        HistogramFiller cFiller(m_vecHist.at(0), m_vecHist.at(1), cEvent);
                        pBoard.accept( cFiller );

                        if ( cN % 100 == 0 )
                            //UpdateHists();
                            emit sendGraphData(m_vecHist);

                        cN++;

                        if ( cN < cTotalEvents )
                            cEvent = fBeBoardInterface->GetNextEvent( &pBoard );
                        else break;
                    }
                    cNthAcq++;
                } // End of Analyze Events of last Acquistion loop
            }
        }
        m_vecHist.at(0)->Scale( 100 / double_t( cTotalEvents ) );
        m_vecHist.at(0)->GetYaxis()->SetRangeUser( 0, 100 );
        m_vecHist.at(1)->Scale( 100 / double_t( cTotalEvents ) );
        m_vecHist.at(1)->GetYaxis()->SetRangeUser( 0, 100 );
        emit sendGraphData(m_vecHist);
        //UpdateHists();
    }

}