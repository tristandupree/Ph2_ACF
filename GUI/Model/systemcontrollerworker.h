#pragma once
#include <QObject>
#include <QMutex>
#include "Model/settings.h"
#include <QVariantMap>

#include "../HWInterface/BeBoardFWInterface.h"
#include "../HWDescription/Shelve.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../Utils/Visitor.h"

#include <iostream>
#include <vector>
#include <map>
#include <stdlib.h>
#include <string.h>

#include "../tools/Tool.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

namespace GUI
{
    class SystemControllerWorker : public QObject, public Tool
    {
        Q_OBJECT
    public:

    public:
        explicit SystemControllerWorker(QObject *parent,
                                        Settings &config);
        ~SystemControllerWorker();

        void requestWork(std::string cHwFile);
        void requestConfigureHw();
        void abort();

    signals:
        void workRequested();
        void workConfigureHwRequested();
        void finishedInitialiseHw();
        void finishedConfigureHw();

    public slots:
        void onInitialiseHw();
        void onConfigureHw();

    private:

        std::string m_HwFile;

        bool _abort;
        bool _working;
        QMutex mutex;

        explicit SystemControllerWorker(const SystemControllerWorker& rhs) = delete;
        SystemControllerWorker& operator= (const SystemControllerWorker& rhs) = delete;
    };
}
