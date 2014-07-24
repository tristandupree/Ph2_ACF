/*

  \file                        CbcInterface.h
  \brief                       CbcInterface class, config of the Cbcs
  \author                      Nicolas PIERRE
  \version :                   0.3
  Date of creation :           07/06/14
  Support :                    mail to : nicolas.pierre@cern.ch

*/

#ifndef __CBCINTERFACE_H__
#define __CBCINTERFACE_H__
#include <time.h>
#include <string>
#include <TROOT.h>
#include <limits.h>
#include <boost/cstdint.hpp>
#include "RegManager.h"
#include "../HWDescription/CbcRegItem.h"
#include "../HWDescription/Cbc.h"
#include "../HWDescription/Module.h"

using namespace Ph2_HwDescription;

/*!
* \namespace Ph2_HwInterface
* \brief Namespace regrouping all the interfaces to the hardware
*/
namespace Ph2_HwInterface
{
    /*!
    * \class CbcInterface
    * \brief Permit r/w given registers in the Cbc you specify
    */
    class CbcInterface : public RegManager
    {
        private:
            std::string fStrSram, fStrOtherSram, fStrSramUserLogic, fStrFull, fStrReadout;

        private:
            void SelectSRAM(uint32_t pCbcId);
            /*!
            * \brief Wait for the I2C command acknowledgement
            * \param pAckVal : Expected status of acknowledgement, 1/0 -> true/false
            * \param pNcount : Number of registers at stake
            * \return boolean confirming the acknowledgement
            */
    		bool I2cCmdAckWait( uint32_t pAckVal, uint8_t pNcount=1 );
            /*!
            * \brief Send request to r/w blocks via I2C
            * \param pVecReq : Block of words to send
            * \param pWrite : 1/0 -> Write/Read
            */
            void SendBlockCbcI2cRequest(std::vector<uint32_t>& pVecReq, bool pWrite);
            /*!
            * \brief Read blocks from SRAM via I2C
            * \param pVecReq : Vector to stack the read words
            */
            void ReadI2cBlockValuesInSRAM(std::vector<uint32_t> &pVecReq );
            /*!
            * \brief Enable I2C communications
            * \param pCbc : Cbc to work with
            * \param pEnable : 1/0 -> Enable/Disable
            */
            void EnableI2c( Cbc* pCbc, bool pEnable );

            //r/w the Cbc registers
            /*!
            * \brief Write register blocks of a Cbc
            * \param pCbc : Cbc to work with
            * \param pVecReq : Block of words to write
            */
            void WriteCbcBlockReg( Cbc* pCbc, std::vector<uint32_t>& pVecReq );
            /*!
            * \brief Read register blocks of a Cbc
            * \param pCbc : Cbc to work with
            * \param pVecReq : Vector to stack the read words
            */
            void ReadCbcBlockReg( Cbc* pCbc, std::vector<uint32_t>& pVecReq );

            //Encode/Decode Cbc values
            /*!
            * \brief Encode a/several word(s) readable for a Cbc
            * \param pRegItem : RegItem containing infos (name, adress, value...) about the register to write
            * \param pCbcId : Id of the Cbc to work with
            * \param pVecReq : Vector to stack the encoded words
            */
            void EncodeReg(CbcRegItem& pRegItem, uint8_t& pCbcId, std::vector<uint32_t>& pVecReq); /*!< Encode a/several word(s) readable for a Cbc*/
            /*!
            * \brief Decode a word from a read of a register of the Cbc
            * \param pRegItem : RegItem containing infos (name, adress, value...) about the register to read
            * \param pCbcId : Id of the Cbc to work with
            * \param pWord : variable to put the decoded word
            */
            void DecodeReg(CbcRegItem& pRegItem, uint8_t& pCbcId, uint32_t pWord); /*!< Decode a word from a read of a register of the Cbc*/

        public:
            /*!
            * \brief Constructor of the CbcInterface class
            * \param puHalConfigFileName : path of the uHal Config File
            */
            CbcInterface(const char *puHalConfigFileName);
            /*!
            * \brief Destructor of the CbcInterface class
            */
            ~CbcInterface();

            /*!
            * \brief Configure the Cbc with the Cbc Config File
            * \param pCbc
            */
            void ConfigureCbc(Cbc* pCbc);
            /*!
            * \brief Write the designated register in both Cbc and Cbc Config File
            * \param pCbc
            * \param pRegNode : Node of the register to write
            * \param pValue : Value to write
            */
            void UpdateCbcWrite(Cbc* pCbc, const std::string& pRegNode, uint8_t pValue);
            /*!
            * \brief Read the designated register in the Cbc and update the Cbc Config File
            * \param pCbc
            * \param pRegNode : Node of the register to read
            */
            void UpdateCbcRead(Cbc* pCbc,const std::string& pRegNode);
            /*!
            * \brief Read same register in all Cbcs and then UpdateCbcRead
            * \param pModule : Module containing vector of Cbcs
            * \param pRegNode : Node of the register to read
            */
            void ReadCbc(Module* pModule,const std::string& pRegNode);
            /*!
            * \brief Write same register in all Cbcs and then UpdateCbcRead
            * \param pModule : Module containing vector of Cbcs
            * \param pRegNode : Node of the register to write
            * \param pValue : Value to write
            */
            void WriteBroadcast(Module* pModule,const std::string& pRegNode,uint8_t pValue);
            /*!
            * \brief Hard reset of the Cbc
            * \param pCbc
            */
            void CbcHardReset(Cbc* pCbc);
            /*!
            * \brief Fast Reset of the Cbc
            * \param pCbc
            */
            void CbcFastReset(Cbc* pCbc);

    };
}

#endif
