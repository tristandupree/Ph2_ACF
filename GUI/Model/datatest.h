#pragma once
#include <QObject>
#include <QVector>
#include "TH1D.h"

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
        void sendGraphData(const std::vector<TH1D*> graphs);

    public slots:

        void relaySendGraphData(const std::vector<TH1D *> graphs);

        void createGraph();

    private:

        SystemController&m_systemController;
        QThread *m_thread;
        DataTestWorker *m_worker;

        //void runDataTest();

        explicit DataTest(const DataTest& rhs) = delete;
        DataTest& operator= (const DataTest& rhs) = delete;

        void WireThreadConnections();
    };
}



