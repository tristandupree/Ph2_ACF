#pragma once
#include <QObject>

namespace GUI{
    class cbcregisterworker : public QObject
    {
        Q_OBJECT
    public:
        explicit cbcregisterworker(QObject *parent=0);

    private:
        explicit cbcregisterworker(const cbcregisterworker& rhs) = delete;
        cbcregisterworker& operator= (const cbcregisterworker& rhs) = delete;
    };
}



