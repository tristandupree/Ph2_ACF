#pragma once
#include <QObject>
#include "lib/qcustomplot.h"
#include <QVector>

namespace GUI{

    class SystemController;

    class DataTest : public QObject
    {
        Q_OBJECT
    public:
        explicit DataTest(QObject *parent,
                          SystemController& sysCtrl);

        ~DataTest();

    signals:
        void sendGraph(QCPBars* plot);

    public slots:

        void createGraph();

        QCPBars *runTest();



    private:

        SystemController& m_systemController;

        explicit DataTest(const DataTest& rhs) = delete;
        DataTest& operator= (const DataTest& rhs) = delete;
    };
}



