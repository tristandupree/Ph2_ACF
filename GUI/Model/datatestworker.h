#define _GUI
#pragma once

#include "../HWDescription/Module.h"
#include "../HWDescription/Cbc.h"
#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../Utils/ConsoleColor.h"
#include "../Utils/Visitor.h"
#include "../Utils/Utilities.h"
#include "../Utils/CommonVisitors.h"

#include <vector>

#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TStyle.h"
#include "TLine.h"
#include "TH1F.h"

#ifdef _GUI
#include <QDebug>
#include <QThread>
#include <QObject>
#include <QVector>
#include <QMutex>
#include <QVariantMap>
#endif



#include "Model/systemcontroller.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

#ifdef _GUI
namespace GUI{
#endif

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

        void Initialise();
        void InitialiseHists();

        void ReadDataTest();
        void ScanThreshold();
        void Measure();
        uint32_t fNCbc;   /*!< Number of CBCs in the Setup */
#ifdef _GUI
        int m_Vcth;
        int m_Events;
        bool m_scan;
        bool m_test;
        bool m_HoleMode;
        std::vector<TCanvas *> m_canvas;

        SystemController& m_systemController;
        std::vector<std::shared_ptr<TH1F>> m_vecHist;
        std::vector<TH1F*> m_hist;

        std::vector<std::shared_ptr<TH1F>> m_vecSCurve;
        std::vector<std::shared_ptr<TF1>> m_vecFit;
        std::vector<std::shared_ptr<TLine>> m_vecTLine;

        /*void draw(std::string type)
        {
            if (type="Occupancy")
            {
              emit sendOccupyHists(m_vecHist);
            }

            if (type="Threshold")
            {
              emit sendHistsThreshold(m_vecSCurve, "P");
            }

            if (type="FitThreshold")
            {
              emit sendFitThreshold(m_vecFit, "same");
            }
        }*/


        explicit DataTestWorker(const DataTestWorker& rhs) = delete;
        DataTestWorker& operator= (const DataTestWorker& rhs) = delete;
#endif
    };
#ifdef _GUI
}
#endif



