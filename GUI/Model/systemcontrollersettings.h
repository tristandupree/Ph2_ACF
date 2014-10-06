#pragma once
#include "../HWInterface/BeBoardFWInterface.h"
#include "../HWDescription/Shelve.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

namespace GUI{
    struct SystemControllerSettings
    {
        typedef std::vector<Shelve*> ShelveVec;
        typedef std::map<std::string,uint8_t> SettingsMap;

        BeBoardInterface*	fBeBoardInterface; /*!< Interface to the BeBoard */
        CbcInterface*		fCbcInterface; /*!< Interface to the Cbc */
        ShelveVec		 	fShelveVec; /*!< Vector of Shelve pointers */
        BeBoardFWMap   		fBeBoardFWMap; /*!< Map of connections to the BeBoard */
        SettingsMap			fSettingsMap; /*!< Maps the settings */
    };
}



