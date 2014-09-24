#pragma once
#include <QObject>

namespace GUI{
    class CbcRegViewManager : public QObject
    {
        Q_OBJECT
    public:
        explicit CbcRegViewManager(QObject *parent);

    ~CbcRegViewManager();

    private:
        explicit CbcRegViewManager(const CbcRegViewManager& rhs) = delete;
        CbcRegViewManager& operator= (const CbcRegViewManager& rhs) = delete;
        void createGrid();
    };
}



