#pragma once
#include <QObject>
#include "Model/systemcontroller.h"
#include <map>

#include "cbcregisterworker.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

namespace GUI{
    class SystemController;

    class CbcRegisters : public QObject
    {
        Q_OBJECT
    public:
        explicit CbcRegisters(QObject *parent,
                              SystemController &sysCtrl);

        void requestWork();
        void abort();

        ~CbcRegisters();

    signals:
        void sendCbcRegisterValue(const int cbc, const std::map<std::string, CbcRegItem> mapReg);

    public slots:
        void getCbcRegistersMap();
        void sendCbcRegisters(const int cbc, std::vector<std::pair<std::string, std::uint8_t>> mapReg);

    private:
        SystemController& m_systemController;

        BeBoardInterface*       fBeBoardInterface;
        CbcInterface*           fCbcInterface;
        ShelveVec fShelveVector;
        BeBoardFWMap fBeBoardFWMap;

        QThread *m_thread;
        cbcregisterworker *m_worker;

        void getObjects();

        void WireThreadConnections();

        explicit CbcRegisters(const CbcRegisters& rhs) = delete;
        CbcRegisters& operator= (const CbcRegisters& rhs) = delete;
    };
}



