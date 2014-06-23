/*

  \file 					     CbcInterface.h
  \brief 					    CbcInterface class, config of the Cbcs
  \author 				       Nicolas PIERRE
  \version : 					1.3
  Date of creation : 	        07/06/14
  Support : 					 mail to : nicolas.pierre@cern.ch

*/

#ifndef __CBCINTERFACE_H__
#define __CBCINTERFACE_H__
#include <time.h>
#include <string>
#include <TROOT.h>
#include <limits.h>
#include <boost/cstdint.hpp>
#include "RegManager.h"
#include "../../HWDescription/Description/CbcRegItem.h"
#include "../../HWDescription/Description/Cbc.h"
#include "../../HWDescription/Description/Module.h"

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
            //I2C functions to handle with the Cbc
            void SelectSramForI2C( uint8_t pCbcId ); /*!< Select SRAM for I2C com*/
    		bool I2cCmdAckWait( uint32_t pAckVal, uint8_t pNcount=1 ); /*!< Wait for the I2C command acknowledgement*/
            void SendBlockCbcI2cRequest( uint32_t pCbcId, std::vector<uint32_t>& pVecReq, bool pWrite); /*!< Send request to r/w blocks via I2C*/
            void ReadI2cBlockValuesInSRAM( unsigned int pCbcId, std::vector<uint32_t> &pVecReq ); /*!< Read blocks from SRAM via I2C*/
            void EnableI2c( Cbc& pCbc, bool pEnable ); /*!< Enable I2C com*/

            //r/w the Cbc registers
            void WriteCbcBlockReg( Cbc& pCbc, std::vector<uint32_t>& pVecReq ); /*!< Write register blocks of a Cbc*/
            void ReadCbcBlockReg( Cbc& pCbc, std::vector<uint32_t>& pVecReq ); /*!< Read register blocks of a Cbc*/

            //Encode/Decode Cbc values
            void EncodeReg(CbcRegItem& pRegItem, uint8_t& pCbcId, std::vector<uint32_t> pVecReq); /*!< Encode a/several word(s) readable for a Cbc*/
            void DecodeReg(CbcRegItem& pRegItem, uint8_t& pCbcId, uint32_t pWord); /*!< Decode a word from a read of a register of the Cbc*/

        public:
            static const std::string fStrI2cSettings;
			static const std::string fStrI2cCommand;
			static const std::string fStrI2cReply;
			static const uint32_t fI2cSlave;

        public:
            CbcInterface(const char *puHalConfigFileName);
            ~CbcInterface();

            void ConfigureCbc(Cbc& pCbc); /*!< Configure the Cbc with the Cbc Config File*/
            void UpdateCbcWrite(Cbc& pCbc, const std::string& pRegNode, uint8_t pValue); /*!< Write the designated register in both Cbc and Cbc Config File*/
            void UpdateCbcRead(Cbc& pCbc,const std::string& pRegNode); /*!< Read the designated register in the Cbc and update the Cbc Config File */
            void ReadCbc(Module& pModule,const std::string& pRegNode); /*!< Read same register in all Cbcs and then UpdateCbcRead */
            void WriteBroadcast(Module& pModule,const std::string& pRegNode,uint32_t& pWord); /*!< Write same register in all Cbcs and then UpdateCbcWrite */

    };
}

#endif
