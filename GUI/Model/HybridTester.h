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
#include "Model/systemcontroller.h"

#else
#include "../System/SystemController.h"
using namespace Ph2_System;
#endif

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

#ifdef _GUI
namespace GUI{

    class HybridTester : public QObject
    {
        Q_OBJECT
    public:
        explicit HybridTester(QObject *parent,
                                SystemController &sysController);

        void requestWork(int cVcth,
                         int cEvents,
                         bool testReg,
                         bool scanReg,
                         bool holeMode);
        void abort();

#else
class HybridTester : public SystemController
{
     void InitialiseSettings();
#endif

        void ReadDataTest();
        void ScanThreshold();
        void Measure();
        void TestRegisters();

        ~HybridTester();

#ifdef _GUI
    signals:
        void workRequested();
        void finished();
        void sendAccept(HwDescriptionVisitor pVisitor); //not working

        void sendOccupyHists(const std::vector<std::shared_ptr<TH1F>> graph);
        void sendHistsThreshold(const std::vector<std::shared_ptr<TH1F>> graph, std::string option);
        void sendFitThreshold(const std::vector<std::shared_ptr<TF1>> graph, std::string option);
        void sendHists(const std::map<Cbc*, TH1F*> graph, std::string option);
        void sendRefreshHists();

    public slots:
        void doWork();
#endif

    private:

        BeBoardInterface*       fBeBoardInterface;
        CbcInterface*           fCbcInterface;
        ShelveVec fShelveVector;
        BeBoardFWMap fBeBoardFWMap;

        void Initialise();
        void InitialiseHists();
        void processSCurves( uint32_t pEventsperVcth );

        uint32_t fNCbc;   /*!< Number of CBCs in the Setup */
        std::map<Cbc*, TH1F*> fSCurveMap;  /*!< Histograms for SCurve */
        std::map<Cbc*, TF1*> fFitMap;   /*!< fits for SCurve*/

        int m_Vcth;
        int m_TotalEvents;
        bool m_scan;
        bool m_test;
        bool m_HoleMode;
        int m_Sigmas;

        int debug = 0;

#ifdef _GUI

        SystemController& m_systemController;
        std::vector<std::shared_ptr<TH1F>> m_vecHistTopBottom;

        std::shared_ptr<TH1F>  fHistTop;
        std::shared_ptr<TH1F>  fHistBottom;
        void accept( HwDescriptionVisitor& pVisitor );

        void UpdateHists()
        {
            emit sendRefreshHists();
        }

        explicit HybridTester(const HybridTester& rhs) = delete;
        HybridTester& operator= (const HybridTester& rhs) = delete;
#else
        void UpdateHists()
        {
            fDataCanvas->cd( 1 );
            fHistTop->Draw();
            fDataCanvas->cd( 2 );
            fHistBottom->Draw();
            fDataCanvas->Update();
        }
#endif
    };
#ifdef _GUI
}
#endif
