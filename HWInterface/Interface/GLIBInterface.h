/*

    FileName :                    GlibInterface.h
    Content :                     GlibInterface class, init/config of the Glib
    Programmer :                  Nicolas PIERRE
    Version :                     1.3
    Date of creation :            07/06/14
    Support :                     mail to : nicolas.pierre@cern.ch

*/
#ifndef __GLIBINTERFACE_H__
#define __GLIBINTERFACE_H__

#include <string>
#include <map>
#include <vector>
#include <uhal/uhal.hpp>
#include "RegManager.h"
#include "../../HWDescription/Description/Glib.h"

using namespace Ph2_HwDescription;

namespace Ph2_HwInterface
{
    class GlibInterface : public RegManager
    {
        private:
			//Unused variables for the moment, useful for the future
            /*
            unsigned int                    fNeventPerAcq;
            unsigned int                    fNTotalAcq;
			bool                            fNegativeLogicCBC;
			bool                            fStop;
            */

            struct timeval fStartVeto;
            std::string fStrSram, fStrSramUserLogic, fStrFull, fStrReadout;

            uhal::ValVector<uint32_t> fData;

        private:
            //Select the SRAM for DAQ
            void SelectSRAM(uint32_t pNthAcq);

        public:
            //Constructor, takes a Glib object as parameter
            GlibInterface(const char *puHalConfigFileName);
            //Destructor
            ~GlibInterface();

            //Configure the Glib w or w/o a Glib object as parameter
            void ConfigureGlib(Glib& pGlib);
            //Start an acq
            void Start(Glib& pGlib);
            //Stop an acq
            void Stop(Glib& pGlib,uint32_t pNthAcq);
            //(Un)pause the acq
            void Pause(Glib& pGlib);
            void Unpause(Glib& pGlib);
            //Read Data from acq
            void ReadData(Glib& pGlib,uint32_t pNthAcq,bool pBreakTrigger);
            //Write the designated register in both Glib and GlibConfigFile
            void UpdateGlibWrite(Glib& pGlib,const std::string& pRegNode,const uint32_t& pVal);
            //Read the designated register in the Glib and update the GlibConfigFile
            void UpdateGlibRead(Glib& pGlib,const std::string& pRegNode);
            
    };
}

#endif
