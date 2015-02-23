#pragma once

#include <QObject>


namespace GUI{

    class CmTestWorker;

    class CmTest : public QObject
    {
        Q_OBJECT
    public:
        explicit CmTest(QObject *parent);
        ~CmTest();

    signals:

    public slots:


    private:

        QThread *m_thread;
        CmTestWorker *m_worker;

        void WireThreadConnections();


        explicit CmTest(const CmTest& rhs) = delete;
        CmTest& operator= (const CmTest& rhs) = delete;
    };

}
