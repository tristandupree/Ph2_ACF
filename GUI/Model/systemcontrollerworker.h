#pragma once
#include <QObject>

namespace GUI{
    class SystemControllerWorker : public QObject
    {
        Q_OBJECT
    public:
        explicit SystemControllerWorker(QObject *parent=0);

    private:
        explicit SystemControllerWorker(const SystemControllerWorker& rhs) = delete;
        SystemControllerWorker& operator= (const SystemControllerWorker& rhs) = delete;
    };
}



