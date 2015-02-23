#pragma once

#include <QObject>

namespace GUI{
    class CmTestWorker : public QObject
    {
        Q_OBJECT
    public:
        explicit CmTestWorker(QObject *parent);
        ~CmTestWorker();

    signals:

    public slots:

    private:
        explicit CmTestWorker(const CmTestWorker& rhs) = delete;
        CmTestWorker& operator= (const CmTestWorker& rhs) = delete;

    };
}

