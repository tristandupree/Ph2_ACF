#include "HybridTester.h"

#include "utils.h"
#include "unistd.h"


#ifdef _GUI //!!SETS UP GUI THREAD
namespace GUI
{

    HybridTester::HybridTester(QObject *parent,
                               SystemController &sysController) :
        QObject(parent),
        m_systemController(sysController)
    {
    }

    HybridTester::~HybridTester()
    {
        qDebug() << "Destructing " << this;
    }

    void HybridTester::requestWork(int cVcth, int cEvents,
                                   bool testReg, bool scanReg, bool holeMode)
    {
        m_Vcth = cVcth;
        m_TotalEvents = cEvents;
        m_test = testReg;
        m_scan = scanReg;
        m_HoleMode = holeMode;

        qDebug()<<"Request worker start in Thread "<<thread()->currentThreadId();

        emit workRequested();
    }

    void HybridTester::abort()
    {
        //TODO
        /*mutex.lock();
        if (_working) {
            _abort = true;
            qDebug()<<"Request worker aborting in Thread "<<thread()->currentThreadId();
        }
        mutex.unlock();*/
    }

    void HybridTester::doWork()
    {
        fBeBoardInterface = m_systemController.getBeBoardInterface();
        fCbcInterface = m_systemController.getCbcInterface();
        fShelveVector = m_systemController.getfShelveVector();
        fBeBoardFWMap = m_systemController.getBeBoardFWMap();

        CbcRegWriter cWriter (fCbcInterface, "VCth", m_Vcth);
        m_systemController.m_worker->accept(cWriter); //TODO pass safe

        Initialise();

        if(m_test)
        {
            TestRegisters();
        }

        if(m_scan)
        {

            ScanThreshold();
        }

        Measure();

        emit finished();

    }
#endif

    struct HistogramFiller  : public HwDescriptionVisitor
    {
        std::shared_ptr<TH1F>  fBotHist;
        std::shared_ptr<TH1F>  fTopHist;
        const Event* fEvent;

        HistogramFiller( std::shared_ptr<TH1F> pBotHist, std::shared_ptr<TH1F> pTopHist, const Event* pEvent ): fBotHist( pBotHist ), fTopHist( pTopHist ), fEvent( pEvent ) {}

        void visit( Cbc& pCbc ) {
            std::vector<bool> cDataBitVector = fEvent->DataBitVector( pCbc.getFeId(), pCbc.getCbcId() );
            for ( uint32_t cId = 0; cId < NCHANNELS; cId++ ) {
                if ( cDataBitVector.at( cId ) ) {
                    uint32_t globalChannel = ( pCbc.getCbcId() * 254 ) + cId;
                    //              std::cout << "Channel " << globalChannel << " VCth " << +pCbc.getReg( "VCth" ) << std::endl;
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
            for ( uint32_t cId = 0; cId < NCHANNELS; cId++ ) {
                if ( fEvent->DataBit( pCbc.getFeId(), pCbc.getCbcId(), cId ) ) fHitcounter++;
            }
        }
    };

    void HybridTester::Initialise()
    {
        //  special Visito class to count objects
        Counter cCbcCounter;
        m_systemController.m_worker->accept(cCbcCounter);
        fNCbc = cCbcCounter.getNCbc();
#ifndef _GUI
        InitialiseSettings();
#endif

        InitialiseHists();

    }

    void HybridTester::InitialiseHists()
    {
        m_vecHistTopBottom.clear();
        TString cFrontName( "fHistTop" );

        fHistTop = std::make_shared<TH1F>(cFrontName, "Front Pad Channels; Pad Number; Occupancy [%]", ( fNCbc / 2 * 254 ) + 1, -0.5, ( fNCbc / 2 * 254 ) + .5);
        fHistTop->SetFillColor(4);
        fHistTop->SetFillStyle(3001);

        TString cBackName( "fHistBottom" );
        fHistBottom = std::make_shared<TH1F>( cBackName, "Back Pad Channels; Pad Number; Occupancy [%]", ( fNCbc / 2 * 254 ) + 1 , -0.5, ( fNCbc / 2 * 254 ) + .5 );
        fHistBottom->SetFillColor(4);
        fHistBottom->SetFillStyle(3001);

        m_vecHistTopBottom.push_back(fHistTop);
        m_vecHistTopBottom.push_back(fHistBottom);

        emit sendOccupyHists(m_vecHistTopBottom);

        // Now the Histograms for SCurves
        for ( auto cShelve : fShelveVector )
        {
            uint32_t cShelveId = cShelve->getShelveId();

            for ( auto cBoard : cShelve->fBoardVector )
            {
                uint32_t cBoardId = cBoard->getBeId();

                for ( auto cFe : cBoard->fModuleVector )
                {
                    uint32_t cFeId = cFe->getFeId();

                    for ( auto cCbc : cFe->fCbcVector )
                    {
                        uint32_t cCbcId = cCbc->getCbcId();

                        TString cName = Form( "SCurve_Fe%d_Cbc%d", cFeId, cCbcId );
                        TObject* cObject = static_cast<TObject*>( gROOT->FindObject( cName ) );
                        if ( cObject ) delete cObject;
                        TH1F* cTmpScurve = new TH1F( cName, Form( "Noise Occupancy Cbc%d; VCth; Counts", cCbcId ), 255, 0, 255 );
                        cTmpScurve->SetMarkerStyle( 8 );
                        fSCurveMap[cCbc] = cTmpScurve;

                        cName = Form( "SCurveFit_Fe%d_Cbc%d", cFeId, cCbcId );
                        cObject = static_cast<TObject*>( gROOT->FindObject( cName ) );
                        if ( cObject ) delete cObject;
                        TF1* cTmpFit = new TF1( cName, MyErf, 0, 255, 2 );
                        fFitMap[cCbc] = cTmpFit;
                    }
                }
            }
        }

    }

#ifndef _GUI
    void HybridTester::InitialiseSettings()
    {
        auto cSetting = fSettingsMap.find( "HoleMode" );
        m_HoleMode = ( cSetting != std::end( fSettingsMap ) ) ? cSetting->second : true;

        auto cSetting = fSettingsMap.find( "Nevents" );
        m_TotalEvents = ( cSetting != std::end( fSettingsMap ) ) ? cSetting->second : 200;

        auto cSetting = fSettingsMap.find( "Threshold_NSigmas" );
        m_Sigmas = ( cSetting != std::end( fSettingsMap ) ) ? cSetting->second : 4;
    }
#endif

    void HybridTester::ScanThreshold()
    {
        bool cHoleMode = m_HoleMode;

        std::cout << "Scanning noise Occupancy to find threshold for test with external source ... " << std::endl;

        // Necessary variables
        uint32_t cEventsperVcth = 10;
        bool cNonZero = false;
        bool cAllOne = false;
        bool cSlopeZero = false;
        uint32_t cAllOneCounter = 0;
        uint32_t cSlopeZeroCounter = 0;
        uint32_t cOldHitCounter = 0;
        uint8_t  cDoubleVcth;
        uint8_t cVcth = ( cHoleMode ) ?  0xFF :  0x00;
        int cStep = ( cHoleMode ) ? -10 : 10;


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

#ifdef _GUI
            m_systemController.m_worker->accept(cWriter);
#else
            accept( cWriter );
#endif

            uint32_t cN = 0;
            uint32_t cNthAcq = 0;
            uint32_t cHitCounter = 0;

            // maybe restrict to pBoard? instead of looping?
            for ( auto& cShelve : fShelveVector )
            {
                if ( cAllOne ) break;
                for ( BeBoard* pBoard : cShelve->fBoardVector )
                {
                    while ( cN <  cEventsperVcth )
                    {
#ifdef _GUI
                        m_systemController.m_worker->Run( pBoard, cNthAcq );
#else
                        Run( pBoard, cNthAcq );
#endif

                        const Event* cEvent = fBeBoardInterface->GetNextEvent( pBoard );

                        // Loop over Events from this Acquisition
                        while ( cEvent )
                        {
                            if ( cN == cEventsperVcth )
                                break;

                            // loop over Modules & Cbcs and count hits separately
                            ///cHitCounter += fillSCurves( pBoard,  cEvent, cVcth );
                            cN++;

                            if ( cN < cEventsperVcth )
                                cEvent = fBeBoardInterface->GetNextEvent( pBoard );
                            else break;
                        }
                        cNthAcq++;
                    }

                    // Draw the thing after each point
                    ///updateSCurveCanvas( pBoard );

                    // check if the hitcounter is all ones

                    if ( cNonZero == false && cHitCounter != 0 )
                    {
                        cDoubleVcth = cVcth;
                        cNonZero = true;
                        cVcth -= 2 * cStep;
                        cStep /= 10;
                        continue;
                    }
                    if ( cNonZero && cHitCounter != 0 )
                    {
                        // check if all Cbcs have reached full occupancy
                        if ( cHitCounter > 0.95 * cEventsperVcth * fNCbc * NCHANNELS ) cAllOneCounter++;
                        // add a second check if the global SCurve slope is 0 for 10 consecutive Vcth values
                        if ( fabs( cHitCounter - cOldHitCounter ) < 10 && cHitCounter != 0 ) cSlopeZeroCounter++;
                    }
                    if ( cAllOneCounter >= 10 ) cAllOne = true;
                    if ( cSlopeZeroCounter >= 10 ) cSlopeZero = true;

                    if ( cAllOne )
                    {
                        std::cout << "All strips firing -- ending the scan at VCth " << +cVcth << std::endl;
                        break;
                    }
                    else if ( cSlopeZero )
                    {
                        std::cout << "Slope of SCurve 0 -- ending the scan at VCth " << +cVcth << std::endl;
                        break;
                    }

                    cOldHitCounter = cHitCounter;
                    cVcth += cStep;
                }
            }
        }

        // Fit and save the SCurve & Fit - extract the right threshold
        // TODO
        processSCurves( cEventsperVcth );

        // Wait for user to acknowledge and turn on external Source!

        std::cout << "Identified the threshold for 0 noise occupancy - Start external Signal source!" << std::endl;
        //TODO - add external signal source checker
        //mypause();
    }

    void HybridTester::processSCurves( uint32_t pEventsperVcth )
    {
        int cSigmas = m_Sigmas;
        bool cHoleMode = m_HoleMode;

        for ( auto cScurve : fSCurveMap )
        {
#ifndef _GUI
            fSCurveCanvas->cd( cScurve.first->getCbcId() + 1 );
#endif

            cScurve.second->Scale( 1 / double_t( pEventsperVcth * NCHANNELS ) );
#ifndef _GUI
            fSCurveCanvas->cd( cScurve.first->getCbcId() + 1 );
            cScurve.second->Draw( "P" );
#else
            emit sendHists(fSCurveMap, "P");
#endif
            // Write to file
            cScurve.second->Write( cScurve.second->GetName(), TObject::kOverwrite );

            // Estimate parameters for the Fit
            double cFirstNon0( 0 );
            double cFirst1( 0 );

            // Not Hole Mode
            if ( !cHoleMode )
            {
                for ( Int_t cBin = 1; cBin <= cScurve.second->GetNbinsX(); cBin++ )
                {
                    double cContent = cScurve.second->GetBinContent( cBin );
                    if ( !cFirstNon0 )
                    {
                        if ( cContent ) cFirstNon0 = cScurve.second->GetBinCenter( cBin );
                    }
                    else if ( cContent == 1 )
                    {
                        cFirst1 = cScurve.second->GetBinCenter( cBin );
                        break;
                    }
                }
            }
            // Hole mode
            else
            {
                for ( Int_t cBin = cScurve.second->GetNbinsX(); cBin >= 1; cBin-- )
                {
                    double cContent = cScurve.second->GetBinContent( cBin );
                    if ( !cFirstNon0 )
                    {
                        if ( cContent ) cFirstNon0 = cScurve.second->GetBinCenter( cBin );
                    }
                    else if ( cContent == 1 )
                    {
                        cFirst1 = cScurve.second->GetBinCenter( cBin );
                        break;
                    }
                }
            }

            // Get rough midpoint & width
            double cMid = ( cFirst1 + cFirstNon0 ) * 0.5;
            double cWidth = ( cFirst1 - cFirstNon0 ) * 0.5;

            // find the corresponding fit
            auto cFit = fFitMap.find( cScurve.first );
            if ( cFit == std::end( fFitMap ) ) std::cout << "Error: could not find Fit for Cbc " << +cScurve.first->getCbcId() << std::endl;
            else
            {
                // Fit
                cFit->second->SetParameter( 0, cMid );
                cFit->second->SetParameter( 1, cWidth );

                cScurve.second->Fit( cFit->second, "RNQ+" );

                //cFit->second->Draw( "same" );

                // Write to File
                cFit->second->Write( cFit->second->GetName(), TObject::kOverwrite );

                // TODO
                // Set new VCth - for the moment each Cbc gets his own Vcth - I shold add a mechanism to take one that works for all!
                double_t pedestal = cFit->second->GetParameter( 0 );
                double_t noise = cFit->second->GetParameter( 1 );

                uint8_t cThreshold = ceil( pedestal + cSigmas * fabs( noise ) );

                std::cout << "Identified a noise Occupancy of 50% at VCth " << static_cast<int>( pedestal ) << " -- increasing by " << cSigmas <<  " sigmas (=" << fabs( noise ) << ") to " << +cThreshold << " for Cbc " << +cScurve.first->getCbcId() << std::endl;

                //TLine* cLine = new TLine( cThreshold, 0, cThreshold, 1 );
                //cLine->SetLineWidth( 3 );
                //cLine->SetLineColor( 2 );
                //cLine->Draw( "same" );

                fCbcInterface->WriteCbcReg( cScurve.first, "VCth", cThreshold );
            }

        }
        //fSCurveCanvas->Update();

        // Write and Save the Canvas as PDF
        //fSCurveCanvas->Write( fSCurveCanvas->GetName(), TObject::kOverwrite );
        //std::string cPdfName = fDirectoryName + "/NoiseOccupancy.pdf";
        //fSCurveCanvas->SaveAs( cPdfName.c_str() );
    }


    void HybridTester::TestRegisters()
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
        //m_systemController.m_worker->accept( cRegTester );
        cRegTester.dumpResult();
        std::cout << "Done testing registers, re-configuring to calibrated state!" << std::endl;
        m_systemController.m_worker->ConfigureHw();
    }

    void HybridTester::Measure()
    {
        std::cout << "Mesuring Efficiency per Strip ... " << std::endl;

        uint32_t cTotalEvents = m_TotalEvents;
        qDebug() << m_TotalEvents;

        std::cout << "Taking data with " << cTotalEvents << " Events!" << std::endl;

        CbcRegReader cReader( fCbcInterface, "VCth" );

#ifdef _GUI
        m_systemController.m_worker->accept(cReader);
#else
        accept( cReader );
#endif

        for ( auto& cShelve : fShelveVector )
        {
            for ( BeBoard* pBoard : cShelve->fBoardVector )
            {
                uint32_t cN = 0;
                uint32_t cNthAcq = 0;

                while ( cN <  cTotalEvents )
                {
#ifdef _GUI
                    m_systemController.m_worker->Run( pBoard, cNthAcq );
#else
                    Run( pBoard, cNthAcq );
#endif
                    const Event* cEvent = fBeBoardInterface->GetNextEvent( pBoard );
                    // Loop over Events from this Acquisition
                    while ( cEvent )
                    {

                        if ( cN == cTotalEvents )
                            break;
                        HistogramFiller cFiller( fHistBottom, fHistTop, cEvent );
                        pBoard->accept( cFiller );

                        if ( cN % 100 == 0 )
                            UpdateHists();
                        cN++;

                        if ( cN < cTotalEvents )
                            cEvent = fBeBoardInterface->GetNextEvent( pBoard );
                        else break;
                    }
                    cNthAcq++;
                }
            }
        }
        fHistTop->Scale( 100 / double_t( cTotalEvents ) );
        fHistTop->GetYaxis()->SetRangeUser( 0, 100 );
        fHistBottom->Scale( 100 / double_t( cTotalEvents ) );
        fHistBottom->GetYaxis()->SetRangeUser( 0, 100 );

        UpdateHists();
    }



#ifdef _GUI
}
#endif
