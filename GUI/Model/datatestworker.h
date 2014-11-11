#pragma once
#include <QObject>
#include <QVector>
#include <QMutex>
#include <QVariantMap>
//#include <TH1F.h>
#include "TF1.h"
#include "TLine.h"
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
                         bool scanReg,
                         bool holeMode);
        void abort();

        void TestRegisters();

        ~DataTestWorker();

    signals:
        void workRequested();
        void finished();
        void sendAccept(HwDescriptionVisitor pVisitor); //not working

        void sendOccupyHists(const std::vector<std::shared_ptr<TH1F>> graph);
        void sendHistsThreshold(const std::vector<std::shared_ptr<TH1F>> graph, std::string option);
        void sendFitThreshold(const std::vector<std::shared_ptr<TF1>> graph, std::string option);

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
        bool m_HoleMode;
        std::vector<TCanvas *> m_canvas;

        SystemController& m_systemController;
        std::vector<std::shared_ptr<TH1F>> m_vecHist;
        std::vector<TH1F*> m_hist;

        void Initialise();
        void InitialiseHists();

        void ReadDataTest();
        void ScanThreshold();
        void Measure();

        uint32_t fNCbc;   /*!< Number of CBCs in the Setup */

        std::vector<std::shared_ptr<TH1F>> m_vecSCurve;
        std::vector<std::shared_ptr<TF1>> m_vecFit;
        std::vector<std::shared_ptr<TLine>> m_vecTLine;

        explicit DataTestWorker(const DataTestWorker& rhs) = delete;
        DataTestWorker& operator= (const DataTestWorker& rhs) = delete;
    };
}



