#pragma once
#include <QObject>
#include <QVector>
#include <QMutex>
#include <QVariantMap>
#include <TH1F.h>
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

        void requestWork(int cVcth,
                         int cEvents,
                         bool testReg,
                         bool scanReg);
        void abort();

        void TestRegisters();

        ~DataTestWorker();

    signals:
        void workRequested();
        void finished();
        void sendAccept(HwDescriptionVisitor pVisitor); //not working

        void sendGraphData(const std::vector<std::shared_ptr<TH1F>> graph);
        void sendCanvasOccupy(const std::shared_ptr<TCanvas> canvas);

    public slots:
        void doWork();

    private:

        BeBoardInterface*       fBeBoardInterface;
        CbcInterface*           fCbcInterface;
        ShelveVec fShelveVector;
        BeBoardFWMap fBeBoardFWMap;

        int m_Vcth;
        int m_Events;
        bool m_scan;
        bool m_test;
        std::vector<TCanvas *> m_canvas;

        SystemController& m_systemController;
        std::vector<std::shared_ptr<TH1F>> m_vecHist;
        std::vector<TH1F*> m_hist;

        void Initialise(bool pThresholdScan);
        void InitialiseHists();

        void ReadDataTest();
        void ScanThreshold();
        void Measure();

        uint32_t fNCbc;   /*!< Number of CBCs in the Setup */

        //std::shared_ptr<TH1F>  fBotHist;
        //std::shared_ptr<TH1F>  fTopHist;
        TH1F* fSCurve;   /*!< Histogram for SCurve */
        TF1* fFit;   /*!< fit for SCurve*/

        explicit DataTestWorker(const DataTestWorker& rhs) = delete;
        DataTestWorker& operator= (const DataTestWorker& rhs) = delete;
    };
}



