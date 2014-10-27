#pragma once
#include <QObject>

namespace GUI{

    class CbcRegistersTab;
    class CbcRegisters;

    class CbcRegViewManager : public QObject
    {
        Q_OBJECT
    public:
        explicit CbcRegViewManager(QObject *parent,
                                   CbcRegistersTab& cbcRegTab,
                                   CbcRegisters& cbcReg);
    ~CbcRegViewManager();

    signals:
        void on2CbcToggle(const bool);
        void notifyConfigFinished();

    private:
        void WireConnections();
        void WireExternalConnections();

        CbcRegistersTab& m_cbcRegistersTab;
        CbcRegisters& m_cbcRegisters;
        explicit CbcRegViewManager(const CbcRegViewManager& rhs) = delete;
        CbcRegViewManager& operator= (const CbcRegViewManager& rhs) = delete;
    };
}



