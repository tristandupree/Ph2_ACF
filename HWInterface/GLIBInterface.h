/*

    \file                         GlibInterface.h
    \brief                        GlibInterface class, init/config of the Glib
    \author                       Nicolas PIERRE
    \version                      0.3
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
#include "../HWDescription/Glib.h"

using namespace Ph2_HwDescription;

/*!
* \namespace Ph2_HwInterface
* \brief Namespace regrouping all the interfaces to the hardware
*/
namespace Ph2_HwInterface
{
    /*!
    * \class GlibInterface
    * \brief Permit r/w given registers in the Glib you specify
    */
    class GlibInterface : public BeBoardInterface
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

            uhal::ValVector<uint32_t> fData; /*!< Data read storage*/

        private:
            /*!
            * \brief SRAM selection for DAQ
            * \param pNthAcq : actual number of acquisitions
            */
            void SelectSRAM(uint32_t pNthAcq);

        public:
            /*!
            * \brief Constructor of the GlibInterface class
            * \param puHalConfigFileName : path of the uHal Config File
            */
            GlibInterface(const char *puHalConfigFileName);
            /*!
            * \brief Destructor of the GlibInterface class
            */
            ~GlibInterface();

            /*!
            * \brief Configure the Glib with its Config File
            * \param pGlib
            */
            void ConfigureGlib(Glib& pGlib);
            /*!
            * \brief Start a DAQ
            * \param pGlib
            */
            void Start(Glib& pGlib);
            /*!
            * \brief Stop a DAQ
            * \param pGlib
            * \param pNthAcq : actual number of acquisitions
            */
            void Stop(Glib& pGlib,uint32_t pNthAcq);
            /*!
            * \brief Pause a DAQ
            * \param pGlib
            */
            void Pause(Glib& pGlib);
            /*!
            * \brief Unpause a DAQ
            * \param pGlib
            */
            void Unpause(Glib& pGlib);
            /*!
            * \brief Read data from DAQ
            * \param pGlib
            * \param pNthAcq : actual number of acquisitions
            * \param pBreakTrigger : if true, enable the break trigger
            */
            void ReadData(Glib& pGlib,uint32_t pNthAcq,bool pBreakTrigger);
            /*!
            * \brief Update both Glib register and Config File
            * \param pGlib
            * \param pRegNode : Node of the register to update
            * \param pVal : Value to write
            */
            void UpdateGlibWrite(Glib& pGlib,const std::string& pRegNode,const uint32_t& pVal);
            /*!
            * \brief Update Config File with the value in the Glib register
            * \param pGlib
            * \param pRegNode : Node of the register to update
            */
            void UpdateGlibRead(Glib& pGlib,const std::string& pRegNode);

    };
}

#endif
