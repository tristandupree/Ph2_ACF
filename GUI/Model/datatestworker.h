#pragma once
#include <QObject>
#include <QVector>
#include <QMutex>
#include <QVariantMap>
#include <TH1.h>
#include <vector>

#include "Model/systemcontrollersettings.h"

namespace GUI{
    class SystemControllerSettings;
    class DataTestWorker : public QObject
    {
        Q_OBJECT
    public:
        explicit DataTestWorker(QObject *parent,
                                SystemControllerSettings &sysSettings);

        void requestWork();
        void abort();


        ~DataTestWorker();

    signals:
        void workRequested();
        void valueChanged(const QString &value);
        void finished();

        void sendGraphData(const std::vector<TCanvas*> &canvas);

    public slots:

        void doWork();

    private:

        bool _abort;
        bool _working;
        QMutex mutex;
        SystemControllerSettings& m_systemSettings;

        std::vector<TH1F*> cHistVec;

        void ReadDataTest();
        void Run(BeBoard *pBeBoard, uint32_t pNthAcq);



    private:
        explicit DataTestWorker(const DataTestWorker& rhs) = delete;
        DataTestWorker& operator= (const DataTestWorker& rhs) = delete;
    };
}



