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
    class SystemControllerSettings;
    class DataTestWorker : public QObject
    {
        Q_OBJECT
    public:
        explicit DataTestWorker(QObject *parent,
                                SystemController &sysController);

        void requestWork(int cVcth, int cEvents, std::vector<TCanvas *> canvas);
        void abort();


        ~DataTestWorker();

    signals:
        void workRequested();
        void finished();

        void sendGraphData(const std::vector<std::shared_ptr<TH1D>> graph);

    public slots:
        void doWork();

    private:

        BeBoardInterface*       fBeBoardInterface;
        CbcInterface*           fCbcInterface;
        ShelveVec fShelveVector;
        BeBoardFWMap fBeBoardFWMap;

        int m_Vcth;
        int m_Events;
        std::vector<TCanvas *> m_canvas;

        bool _abort;
        bool _working;
        QMutex mutex;
        SystemController& m_systemController;
        std::vector<std::shared_ptr<TH1D>> m_vecHist;
        std::vector<TH1D*> m_hist;

        void ReadDataTest();



    private:
        void Run(BeBoard *pBeBoard, uint32_t pNthAcq);

        explicit DataTestWorker(const DataTestWorker& rhs) = delete;
        DataTestWorker& operator= (const DataTestWorker& rhs) = delete;
    };
}



