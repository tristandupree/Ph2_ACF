#pragma once
#include <QObject>
#include <QVector>
#include "TH1F.h"
#include <QTimer>

#include "datatestworker.h"

namespace GUI{

    class SystemController;
    class DataTestWorker;


    class DataTest : public QObject
    {
        Q_OBJECT
    public:
        explicit DataTest(QObject *parent,
                          SystemController &sysController);

        void requestWork();
        void abort();

        ~DataTest();

    signals:
        void getVcthValue();
        void getEventsValue();
        void getIsRegTestChecked();
        void getIsScanChecked();
        void startedDataTest();
        void finishedDataTest();
        void sendRefresh();
        void sendGraphData(const std::vector<std::shared_ptr<TH1F>> graph);

    public slots:
        void initialiseSettings();
        void setVcthValue(int cVcth);
        void setEventsValue(int cEvents);
        void setTestReg(const bool testReg);
        void setScanThreshold(const bool scanThreshhold);

    private:

        int m_Vcth;
        int m_Events;
        bool m_TestReg;
        bool m_ScanThreshold;

        QTimer *m_timer;

        SystemController &m_systemController;
        QThread *m_thread;
        DataTestWorker *m_worker;

        void WireThreadConnections();
        void WireTimer();

        explicit DataTest(const DataTest& rhs) = delete;
        DataTest& operator= (const DataTest& rhs) = delete;
    };
}



