#include "datatestworker.h"

#include "utils.h"
#include "unistd.h"


#ifdef _GUI
namespace GUI
{

    DataTestWorker::DataTestWorker(QObject *parent,
                                   SystemController &sysController) :
        QObject(parent),
        m_systemController(sysController)
    {
    }

    DataTestWorker::~DataTestWorker()
    {
        qDebug() << "Destructing " << this;
    }

    void DataTestWorker::requestWork(int cVcth, int cEvents,
                                     bool testReg, bool scanReg, bool holeMode)
    {
        m_Vcth = cVcth;
        m_Events = cEvents;
        m_test = testReg;
        m_scan = scanReg;
        m_HoleMode = holeMode;

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

        if(m_scan){

            ScanThreshold();
        }

        Measure();

        //qDebug()<<"Worker process finished in DataTest thread "<<thread()->currentThreadId();
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
                    //std::cout << "Channel " << globalChannel << " VCth " << ( int )pCbc.getReg( "VCth" ) << std::endl;
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

    void DataTestWorker::Initialise()
    {
        //  special Visito class to count objects
        Counter cCbcCounter;
        m_systemController.m_worker->accept(cCbcCounter);
        fNCbc = cCbcCounter.getNCbc();

        InitialiseHists();

    }

    void DataTestWorker::InitialiseHists()
    {
        qDebug() << m_vecHist.size();
        TString cFrontName( "fHistTop" );

        auto h1 = std::make_shared<TH1F>(cFrontName, "Front Pad Channels; Pad Number; Occupancy [%]", ( fNCbc / 2 * 254 ) + 1, -0.5, ( fNCbc / 2 * 254 ) + .5);
        m_vecHist.push_back(h1);
        m_vecHist.at(0)->SetFillColor(4);
        m_vecHist.at(0)->SetFillStyle(3001);

        TString cBackName( "fHistBottom" );
        auto h2 = std::make_shared<TH1F>( cBackName, "Back Pad Channels; Pad Number; Occupancy [%]", ( fNCbc / 2 * 254 ) + 1 , -0.5, ( fNCbc / 2 * 254 ) + .5 );
        m_vecHist.push_back(h2);
        m_vecHist.at(1)->SetFillColor(4);
        m_vecHist.at(1)->SetFillStyle(3001);

        emit sendOccupyHists(m_vecHist);

    }

    void DataTestWorker::ScanThreshold()
    {
        std::cout << "Scanning noise Occupancy to find threshold for test with external source ... " << std::endl;

        // Necessary variables
        uint32_t cEventsperVcth = 10;
        bool cNonZero = false;
        bool cAllOne = false;
        uint32_t cAllOneCounter = 0;
        uint8_t cVcth, cDoubleVcth;
        ( m_HoleMode ) ? cVcth = 0xFF : cVcth = 0x00;
        int cStep = ( m_HoleMode ) ? -10 : 10;

        // Root objects
        auto sCurve  = std::make_shared<TH1F>( "fSCurve", "Noise Occupancy; VCth; Counts", 255, 0, 255 );

        m_vecSCurve.clear();
        m_vecFit.clear();

        m_vecSCurve.push_back(sCurve);
        m_vecSCurve.at(0)->SetMarkerStyle( 8 );

        emit sendHistsThreshold(m_vecSCurve, "P");

        auto fFit  = std::make_shared<TF1>( "fFit", MyErf, 0, 255, 2 );
        m_vecFit.push_back(fFit);

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

                    m_vecSCurve.at(0)->SetBinContent( cVcth, cHitCounter );
                    emit sendHistsThreshold(m_vecSCurve, "P");

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
        m_vecSCurve.at(0)->Scale( 1 / double_t( cEventsperVcth * fNCbc * NCHANNELS ) );
        emit sendHistsThreshold(m_vecSCurve, "P");

        double cFirstNon0( 0 );
        double cFirst1( 0 );

        // Not Hole Mode
        if ( !m_HoleMode )
        {
            for ( Int_t cBin = 1; cBin <= m_vecSCurve.at(0)->GetNbinsX(); cBin++ )
            {
                double cContent = m_vecSCurve.at(0)->GetBinContent( cBin );
                if ( !cFirstNon0 )
                {
                    if ( cContent ) cFirstNon0 = m_vecSCurve.at(0)->GetBinCenter( cBin );
                }
                else if ( cContent == 1 )
                {
                    cFirst1 = m_vecSCurve.at(0)->GetBinCenter( cBin );
                    break;
                }
            }
        }
        // Hole mode
        else
        {
            for ( Int_t cBin = m_vecSCurve.at(0)->GetNbinsX(); cBin >= 1; cBin-- )
            {
                double cContent = m_vecSCurve.at(0)->GetBinContent( cBin );
                if ( !cFirstNon0 )
                {
                    if ( cContent ) cFirstNon0 = m_vecSCurve.at(0)->GetBinCenter( cBin );
                }
                else if ( cContent == 1 )
                {
                    cFirst1 = m_vecSCurve.at(0)->GetBinCenter( cBin );
                    break;
                }
            }
        }

        // Get rough midpoint & width
        double cMid = ( cFirst1 + cFirstNon0 ) * 0.5;
        double cWidth = ( cFirst1 - cFirstNon0 ) * 0.5;

        m_vecFit.at(0)->SetParameter( 0, cMid );
        m_vecFit.at(0)->SetParameter( 1, cWidth );

        m_vecSCurve.at(0)->Fit( m_vecFit.at(0).get(), "RNQ+" );
        emit sendFitThreshold(m_vecFit, "same");

        // Save
        m_vecSCurve.at(0)->Write( m_vecSCurve.at(0)->GetName(), TObject::kOverwrite );
        m_vecFit.at(0)->Write( fFit->GetName(), TObject::kOverwrite );
        //emit save canvas
        //fSCurveCanvas->Write( fSCurveCanvas->GetName(), TObject::kOverwrite );
        std::string fDirectoryName = "output";
        std::string cPdfName = fDirectoryName + "/NoiseOccupancy.pdf";
        //fSCurveCanvas->SaveAs( cPdfName.c_str() );

        // Set new VCth
        double_t pedestal = m_vecFit.at(0)->GetParameter( 0 );
        double_t noise = m_vecFit.at(0)->GetParameter( 1 );

        int cSigmas = 4; //create settings

        uint8_t cThreshold = ceil( pedestal + cSigmas * fabs( noise ) );

        std::cout << "Identified a noise Occupancy of 50% at VCth " << int( pedestal ) << " -- increasing by " << cSigmas <<  " sigmas (" << fabs( noise ) << ") to " << int( cThreshold ) << " for Hybrid test!" << std::endl;

        TLine* cLine = new TLine( cThreshold, 0, cThreshold, 1 );
        cLine->SetLineWidth( 3 );
        cLine->SetLineColor( 2 );
        //cLine->Draw( "same" );
        //fSCurveCanvas->Update();

        CbcRegWriter cWriter( fCbcInterface, "VCth", cThreshold );
        m_systemController.m_worker->accept( cWriter );

        //!No external signal support for now !!!
        // Wait for user to acknowledge and turn on external Source!
        //std::cout << "Identified the threshold for 0 noise occupancy - Start external Signal source!" << std::endl;
        //mypause();*/

        m_vecSCurve.clear();
        m_vecFit.clear();

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
        //m_systemController.m_worker->accept( cRegTester );
        cRegTester.dumpResult();
        std::cout << "Done testing registers, re-configuring to calibrated state!" << std::endl;
        m_systemController.m_worker->ConfigureHw();
    }

    void DataTestWorker::Measure()
    {
        std::cout << "Mesuring Efficiency per Strip ... " << std::endl;
        int cTotalEvents = m_Events;
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

                        HistogramFiller cFiller(m_vecHist.at(0), m_vecHist.at(1), cEvent);

                        pBoard.accept( cFiller );

                        if ( cN % 100 == 0 )
                            emit sendOccupyHists(m_vecHist);

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
        emit sendOccupyHists(m_vecHist);
        m_vecHist.clear();
    }
#ifdef _GUI
}
#endif
