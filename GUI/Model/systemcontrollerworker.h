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
#include "../HWInterface/Visitor.h"

#include <iostream>
#include <vector>
#include <map>
#include <stdlib.h>
#include <string.h>
#include <QMutex>

#include "TFile.h"


using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

namespace GUI
{

    typedef std::vector<Shelve*> ShelveVec;     /*!< Vector of Shelve pointers */
    typedef std::map<std::string, uint32_t> SettingsMap;    /*!< Maps the settings */

    class SystemControllerWorker : public QObject
    {
        Q_OBJECT
    public:
        BeBoardInterface*       fBeBoardInterface;                     /*!< Interface to the BeBoard */
        CbcInterface*           fCbcInterface;                     /*!< Interface to the Cbc */
        ShelveVec fShelveVector;                                           /*!< Vector of Shelve pointers */
        BeBoardFWMap fBeBoardFWMap;                                /*!< Map of connections to the BeBoard */
        SettingsMap fSettingsMap;                                         /*!< Maps the settings */
        std::string fDirectoryName;             /*< the Directoryname for the Root file with results */

    public:
        explicit SystemControllerWorker(QObject *parent,
                                        Settings &config);

        void requestWork();
        void requestConfigureHw();
        void abort();

        void accept( HwDescriptionVisitor& pVisitor ) const {
            pVisitor.visit( *this );
            for ( auto& cShelve : fShelveVector )
                cShelve->accept( pVisitor );
        }

        ~SystemControllerWorker();
        void Run( BeBoard* pBeBoard, uint32_t pNthAcq );

        void ConfigureHw2();
    signals:
        void workRequested();
        void workConfigureHwRequested();
        void finishedInitialiseHw();
        void finishedConfigureHw();

    public slots:
        void InitializeHw();
        void ConfigureHw();

    private:

        bool _abort;
        bool _working;
        QMutex mutex;


        QVariantMap* map_HwDescription;
        Settings& m_Settings;

        explicit SystemControllerWorker(const SystemControllerWorker& rhs) = delete;
        SystemControllerWorker& operator= (const SystemControllerWorker& rhs) = delete;
    };
}
