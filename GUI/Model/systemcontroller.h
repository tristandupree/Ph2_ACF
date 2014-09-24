//NEEDS MASSIVE TIDY

#pragma once
#include <QObject>
#include "../HWInterface/BeBoardFWInterface.h"
#include "../HWDescription/Shelve.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include <iostream>
#include <vector>
#include <map>
#include <stdlib.h>
#include <string.h>

#include <map> //TODO add f/w declare
#include <QVariantMap>

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

namespace GUI{

    class Settings;

    typedef std::vector<Shelve*> ShelveVec;

    typedef std::map<std::string,uint8_t> SettingsMap;

    class SystemController : public QObject
    {
        Q_OBJECT
    public:
        explicit SystemController(QObject *parent,
                                  Settings& config);

        BeBoardInterface*	fBeBoardInterface; /*!< Interface to the BeBoard */
        CbcInterface*		fCbcInterface; /*!< Interface to the Cbc */
        ShelveVec		 	fShelveVec; /*!< Vector of Shelve pointers */
        BeBoardFWMap   		fBeBoardFWMap; /*!< Map of connections to the BeBoard */
        SettingsMap			fSettingsMap; /*!< Maps the settings */

        void Run(BeBoard *pBeBoard, uint32_t pNthAcq);

    ~SystemController();


    signals:
        void notifyStatusMessage(QString msg);

    public slots:

        void startInitialiseHw();
        void startConfigureHw();


    private:

        void InitialiseHw();
        void ConfigureHw();

        uint32_t cShelveId;
        uint32_t cBeId;
        uint32_t cModuleId;
        uint32_t cCbcId;
        uint32_t cFeId;
        uint32_t cFmcId;
        uint32_t cNShelve;


        QVariantMap map_ShelveId; //TODO don't pass in like this

        void SendStatusMessage(QString msg);

        explicit SystemController(const SystemController& rhs) = delete;
        SystemController& operator= (const SystemController& rhs) = delete;

    };
}



