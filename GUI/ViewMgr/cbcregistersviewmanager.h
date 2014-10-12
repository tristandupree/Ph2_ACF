#pragma once
#include <QObject>

namespace GUI{
    class CbcRegistersViewManager : public QObject
    {
        Q_OBJECT
    public:
        explicit CbcRegistersViewManager(QObject *parent=0);

    private:
        explicit CbcRegistersViewManager(const CbcRegistersViewManager& rhs) = delete;
        CbcRegistersViewManager& operator= (const CbcRegistersViewManager& rhs) = delete;
    };
}



