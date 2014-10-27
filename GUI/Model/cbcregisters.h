#pragma once
#include <QObject>
#include "Model/systemcontroller.h"
#include <map>

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

//Q_DECLARE_METATYPE(CbcRegItem)

namespace GUI{
    class SystemController;

    class CbcRegisters : public QObject
    {
        Q_OBJECT
    public:
        explicit CbcRegisters(QObject *parent,
                              SystemController &sysCtrl);

    signals:
        void sendCbcRegisterValue(const int cbc, const std::map<std::string, CbcRegItem> mapReg);

    public slots:
        void getCbcRegistersMap();


    private:
        SystemController& m_systemController;

        BeBoardInterface*       fBeBoardInterface;
        CbcInterface*           fCbcInterface;
        ShelveVec fShelveVector;
        BeBoardFWMap fBeBoardFWMap;

        void getObjects();



        explicit CbcRegisters(const CbcRegisters& rhs) = delete;
        CbcRegisters& operator= (const CbcRegisters& rhs) = delete;
    };
}



