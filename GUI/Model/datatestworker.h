#pragma once
#include <QObject>
#include <QVector>
#include <QMutex>
#include <QVariantMap>
#include <TH1D.h>
#include "TCanvas.h"
#include <vector>

#include "Model/systemcontroller.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

namespace GUI{

    class DataTestWorker : public QObject
    {
        Q_OBJECT
    public:
        explicit DataTestWorker(QObject *parent,
                                SystemController &sysController);

        void requestWork(int cVcth, int cEvents);
        void abort();

        void TestRegisters();

        ~DataTestWorker();

    signals:
        void workRequested();
        void finished();
        void sendAccept(HwDescriptionVisitor pVisitor); //not working

        void sendGraphData(const std::vector<std::shared_ptr<TH1D>> graph);

    public slots:
        void doWork();

    private:

        /*void accept( HwDescriptionVisitor& pVisitor ) const {
            pVisitor.visit( *this );
            for ( auto& cShelve : fShelveVector )
                cShelve->accept( pVisitor );
        }*/

        BeBoardInterface*       fBeBoardInterface;
        CbcInterface*           fCbcInterface;
        ShelveVec fShelveVector;
        BeBoardFWMap fBeBoardFWMap;

        int m_Vcth;
        int m_Events;
        std::vector<TCanvas *> m_canvas;

        SystemController& m_systemController;
        std::vector<std::shared_ptr<TH1D>> m_vecHist;
        std::vector<TH1D*> m_hist;

        void Initialise(bool pThresholdScan);
        void InitialiseHists();

        void ReadDataTest();
        void ScanThreshold();
        void Measure();



    private:
        uint32_t fNCbc;   /*!< Number of CBCs in the Setup */
        TCanvas* fDataCanvas;   /*!<Canvas to output single-strip efficiency */
        TH1F* fHistTop;   /*!< Histogram for top pads */
        TH1F* fHistBottom;   /*!< Histogram for bottom pads */

        //std::shared_ptr<TH1D>  fBotHist;
        //std::shared_ptr<TH1D>  fTopHist;

        TCanvas* fSCurveCanvas;   /*!< Canvas for threshold scan */
        TH1F* fSCurve;   /*!< Histogram for SCurve */
        TF1* fFit;   /*!< fit for SCurve*/

        explicit DataTestWorker(const DataTestWorker& rhs) = delete;
        DataTestWorker& operator= (const DataTestWorker& rhs) = delete;
    };
}



