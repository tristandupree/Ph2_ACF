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
        void sendGraphData(const std::vector<std::shared_ptr<TH1D>> graphs);
        void getVcthValue();
        void getEventsValue();

    public slots:
        void createGraph();
        void setVcthValue(int cVcth);
        void setEventsValue(int cEvents);

    private:

        int m_Vcth;
        int m_Events;

        SystemController&m_systemController;
        QThread *m_thread;
        DataTestWorker *m_worker;

        void WireThreadConnections();

        explicit DataTest(const DataTest& rhs) = delete;
        DataTest& operator= (const DataTest& rhs) = delete;
    };
}



