#pragma once
#include <QObject>
#include <QVector>
#include "TH1F.h"

#include "HybridTester.h"

namespace GUI{

    class SystemController;
    class HybridTester;

    class DataTest : public QObject
    {
        Q_OBJECT
    public:
        explicit DataTest(QObject *parent,
                          SystemController &sysController);

        void abort();

        ~DataTest();

    signals:
        void getVcthValue();
        void getEventsValue();
        void getIsRegTestChecked();
        void getIsScanChecked();
        void getIsHoleModeChecked();
        void startedDataTest();
        void finishedDataTest();
        void sendRefresh();

        void sendOccupancyHists(const std::vector<std::shared_ptr<TH1F>> graph);
        void sendSCurve(const std::map<std::shared_ptr<Cbc>, std::shared_ptr<TH1F>> graph, std::string option);
        void sendSCurve(const std::map<std::shared_ptr<Cbc>, std::shared_ptr<TF1>> graph, std::string option);
        void sendRefreshHists();

    public slots:
        void initialiseSettings();
        void setVcthValue(int cVcth);
        void setEventsValue(int cEvents);
        void setTestReg(const bool testReg);
        void setScanThreshold(const bool scanThreshhold);
        void setHoleMode(const bool holeMode);

    private:

        int m_Vcth;
        int m_Events;
        bool m_TestReg;
        bool m_ScanThreshold;
        bool m_HoleMode;

        SystemController &m_systemController;
        QThread *m_thread;
        HybridTester *m_worker;

        void WireThreadConnections();
        void WireGraphData();

        explicit DataTest(const DataTest& rhs) = delete;
        DataTest& operator= (const DataTest& rhs) = delete;
    };
}



