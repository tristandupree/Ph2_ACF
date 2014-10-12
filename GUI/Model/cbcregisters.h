#pragma once
#include <QObject>

namespace GUI{
    class SystemController;
    //class SystemControllerWorker;
    class CbcRegisters : public QObject
    {
        Q_OBJECT
    public:
        explicit CbcRegisters(QObject *parent,
                              SystemController &sysCtrl);

    private:
        SystemController& m_systemController;
        //ystemControllerWorker& m_systemControllerWorker;

        void getCbcRegistersMap();

        explicit CbcRegisters(const CbcRegisters& rhs) = delete;
        CbcRegisters& operator= (const CbcRegisters& rhs) = delete;
    };
}



