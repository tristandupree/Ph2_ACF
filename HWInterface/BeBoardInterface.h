/*

  FileName :                     BeBoardInterface.h
  Content :                      BeBoardInterface, base class of all type of boards
  Programmer :                   Nicolas PIERRE
  Version :                      0.3
  Date of creation :             01/07/14
  Support :                      mail to : nicolas.pierre@cern.ch

*/

#ifndef __BEBOARDINTERFACE_H__
#define __BEBOARDINTERFACE_H__

#include <string>
#include <map>
#include <vector>
#include <uhal/uhal.hpp>
#include "RegManager.h"


/*!
* \namespace Ph2_HwInterface
* \brief Namespace regrouping all the interfaces to the hardware
*/
namespace Ph2_HwInterface
{

    /*!
    * \class BeBoardInterface
    * \brief
    */
    class BeBoardInterface : public RegManager
    {

        public:

            /*!
            * \brief Constructor of the GlibInterface class
            * \param puHalConfigFileName : path of the uHal Config File
            */
            BeBoardInterface(const char *puHalConfigFileName);
            /*!
            * \brief Destructor of the GlibInterface class
            */
            virtual ~BeBoardInterface();

            /*!
            * \brief Configure the Glib with its Config File
            * \param pGlib
            */
            virtual void ConfigureBoard();
            /*!
            * \brief Start a DAQ
            * \param pGlib
            */
            virtual void Start();
            /*!
            * \brief Stop a DAQ
            * \param pGlib
            * \param pNthAcq : actual number of acquisitions
            */
            virtual void Stop(uint32_t pNthAcq);
            /*!
            * \brief Pause a DAQ
            * \param pGlib
            */
            virtual void Pause();
            /*!
            * \brief Unpause a DAQ
            * \param pGlib
            */
            virtual void Unpause();
            /*!
            * \brief Read data from DAQ
            * \param pGlib
            * \param pNthAcq : actual number of acquisitions
            * \param pBreakTrigger : if true, enable the break trigger
            */
            virtual void ReadData(Glib& pGlib,uint32_t pNthAcq,bool pBreakTrigger) = 0;

    };
}
