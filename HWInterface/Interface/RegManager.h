/*!
    \file                         RegManager.h
    \brief                        RegManager class, permit connection & r/w registers
    \author                       Nicolas PIERRE
    \version                      1.3
    Date of creation :            06/06/14
    Support :                     mail to : nicolas.pierre@cern.ch
*/
#ifndef __REGMANAGER_H__
#define __REGMANAGER_H__

#include <string>
#include <map>
#include <vector>
#include <uhal/uhal.hpp>

/*!
* \namespace Ph2_HwInterface
* \brief Namespace regrouping all the interfaces to the hardware
*/
namespace Ph2_HwInterface
{
    /*!
    * \class RegManager
    * \brief Permit connection to given boards and r/w given registers
    */
    class RegManager
    {
        protected:
            uhal::HwInterface *fBoard; /*!< Board in use*/
            const char *fUHalConfigFileName; /*!< path of the uHal Config File*/
            std::map<uint8_t,uhal::HwInterface*> fBoardMap; /*!< Board Map with all known boards*/

        protected:
            /*!
            * \brief Write a register
            * \param
            * pRegNode : Node of the register to write
            * pVal : Value to write
            */
            virtual bool WriteReg(const std::string& pRegNode, const uint32_t& pVal);
            /*!
            * \brief Write a block of values in a register
            * \param pRegNode : Node of the register to write
            * \param pValues : Block of values to write
            */
            virtual bool WriteBlockReg(const std::string& pRegNode, const std::vector< uint32_t >& pValues);
            /*!
            * \brief Read a value in a register
            * \param pRegNode : Node of the register to read
            * \return ValWord value of the register
            */
            virtual uhal::ValWord<uint32_t> ReadReg(const std::string& pRegNode);
            /*!
            * \brief Read a block of values in a register
            * \param pRegNode : Node of the register to read
            * \param pBlocksize : Size of the block to read
            * \return ValVector block values of the register
            */
            virtual uhal::ValVector<uint32_t> ReadBlockReg(const std::string& pRegNode, const uint32_t& pBlocksize);
            /*!
            * \brief Choose the board we want to talk with
            * \param pBoardId : Id of the Board to connect to
            */
            virtual void ChooseBoard(uint8_t pBoardId);

        public:
            // Connection w uHal
            /*!
            * \brief Constructor of the RegManager class
            * \param puHalConfigFileName : path of the uHal Config File
            */
            RegManager(const char *puHalConfigFileName);
            /*!
            * \brief Destructor of the RegManager class
            */
            virtual ~RegManager();

    };
}

#endif
