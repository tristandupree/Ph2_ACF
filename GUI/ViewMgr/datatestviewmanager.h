#pragma once
#include <QObject>
#include <QThread>

namespace GUI{

    class DataTestTab;
    class DataTest;

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

    private:

        DataTestTab& m_dataTestTab;
        DataTest& m_dataTest;

        void WireButtons();
        void WireCanvas();

        explicit DataTestViewManager(const DataTestViewManager& rhs) = delete;
        DataTestViewManager& operator= (const DataTestViewManager& rhs) = delete;
    };
}



