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
        void createInitialCbcRegistersMap();
        void sendInitialCbcRegisterValue(const int cbc, const std::map<std::string, CbcRegItem> mapReg);
        void sendCbcRegisterValue(const int cbc, const std::map<std::string, CbcRegItem> mapReg);
        void writeCbcRegisterValue(const int cbc, std::vector<std::pair<std::string, std::uint8_t>> mapReg);
        void getCbcRegistersMap();

    private:
        SystemController& m_systemController;

        BeBoardInterface*       fBeBoardInterface;
        CbcInterface*           fCbcInterface;
        ShelveVec fShelveVector;
        BeBoardFWMap fBeBoardFWMap;

        QThread *m_thread;
        CbcRegisterWorker *m_worker;

        void Initialise();

        void WireThreadConnections();

        explicit CbcRegisters(const CbcRegisters& rhs) = delete;
        CbcRegisters& operator= (const CbcRegisters& rhs) = delete;

    };
}



