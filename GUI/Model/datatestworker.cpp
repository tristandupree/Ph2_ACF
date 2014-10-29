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
#include "TH1F.h"

#include "utils.h"
#include "unistd.h"


namespace GUI
{
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
        bool cScan=false;

        fBeBoardInterface = m_systemController.getBeBoardInterface();
        fCbcInterface = m_systemController.getCbcInterface();
        fShelveVector = m_systemController.getfShelveVector();
        fBeBoardFWMap = m_systemController.getBeBoardFWMap();

        CbcRegWriter cWriter (fCbcInterface, "VCth", m_Vcth);
        m_systemController.m_worker->accept(cWriter); //TODO pass safe

        Initialise(cScan);
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

        emit sendGraphData(m_vecHist);

    }

    void DataTestWorker::ScanThreshold()
    {
    }

    void DataTestWorker::TestRegisters()
    {
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
    }

}
