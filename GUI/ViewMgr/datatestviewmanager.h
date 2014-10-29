#pragma once
#include <QObject>
#include <QThread>
#include "Model/datatest.h"

namespace GUI{

    class DataTestTab;
    class DataTest;
    //class HwDescriptionVisitor;

    class DataTestViewManager : public QObject
    {
        Q_OBJECT
    public:
        explicit DataTestViewManager(QObject *parent,
                                     DataTestTab& dataTab,
                                     DataTest& dataTest);
    ~DataTestViewManager();
    signals:
        void on2CbcToggle(const bool);
        void sendAccept(HwDescriptionVisitor pVisitor);

    private:

        DataTestTab& m_dataTestTab;
        DataTest& m_dataTest;

        void WireButtons();
        void WireCanvas();
        void WireExternalCalls();

        explicit DataTestViewManager(const DataTestViewManager& rhs) = delete;
        DataTestViewManager& operator= (const DataTestViewManager& rhs) = delete;
    };
}



