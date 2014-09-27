#pragma once
#include <QObject>
#include <QVector>
#include <QMutex>

namespace GUI{
    typedef QVector<double> MyArray;
    class DataTestWorker : public QObject
    {
        Q_OBJECT
    public:
        explicit DataTestWorker(QObject *parent=0);

        //QThread *m_thread;

        void requestWork();
        void abort();


        ~DataTestWorker();

    signals:
        void workRequested();
        void valueChanged(const QString &value);
        void finished();

        void sendGraphData(const QVector<double> &valueX,
                           const QVector<double> &valueY);

    public slots:

        void doWork();

    private:

        bool _abort;
        bool _working;


        QMutex mutex;

        void ReadDataTest();

        //SystemController& m_systemController;

        //void runDataTest();




    private:
        explicit DataTestWorker(const DataTestWorker& rhs) = delete;
        DataTestWorker& operator= (const DataTestWorker& rhs) = delete;
    };
}



