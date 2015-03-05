#pragma once
#include <QObject>

#include "Model/systemcontroller.h"

namespace GUI{
    class CbcRegisterWorker : public QObject
    {
        Q_OBJECT
    public:
        explicit CbcRegisterWorker(QObject *parent,
                                   SystemController &sysCtrl);
        void requestWork();
        void abort();

        ~CbcRegisterWorker();

    signals:
        void sendCbcRegisterValue(const int cbc, const std::map<std::string, CbcRegItem> mapReg);
        void workRequested();
        void finished();

    public slots:
        void sendCbcRegisters(const int cbc, std::vector<std::pair<std::string, std::uint8_t>> mapReg);
        void doWork();
        void getCbcRegistersMap();

    private:

        SystemController& m_systemController;

        BeBoardInterface*       fBeBoardInterface;
        CbcInterface*           fCbcInterface;
        ShelveVec fShelveVector;
        BeBoardFWMap fBeBoardFWMap;

        void getObjects();

        explicit CbcRegisterWorker(const CbcRegisterWorker& rhs) = delete;
        CbcRegisterWorker& operator= (const CbcRegisterWorker& rhs) = delete;
    };
}



