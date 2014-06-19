/*

  FileName : 					CbcInterface.h
  Content : 					 CbcInterface class, config of the Cbcs
  Programmer : 				  Nicolas PIERRE
  Version : 					 1.3
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

using namespace Ph2_HwDescription;

namespace Ph2_HwInterface
{
    class CbcInterface : public RegManager
    {
        private:
            std::string fStrSram, fStrOtherSram, fStrSramUserLogic, fStrFull, fStrReadout;

        private:
            //I2C functions to handle with the Cbc
            void SelectSramForI2C( uint8_t pCbcId );
    		bool I2cCmdAckWait( uint32_t pAckVal, uint8_t pNcount=1 );
            void SendBlockCbcI2cRequest( uint32_t pCbcId, std::vector<uint32_t>& pVecReq, bool pWrite);
            void ReadI2cBlockValuesInSRAM( unsigned int pCbcId, std::vector<uint32_t> &pVecReq );
            void EnableI2c( Cbc& pCbc, bool pEnable );

            //r/w the Cbc registers
            void WriteCbcBlockReg( Cbc& pCbc, std::vector<uint32_t>& pVecReq );
            void ReadCbcBlockReg( Cbc& pCbc, std::vector<uint32_t>& pVecReq );

            //Encode/Decode Cbc values
            void EncodeReg(CbcRegItem&, uint8_t pCbcId);
            void DecodeReg(uint32_t pWord, CbcRegItem& pRegItem, uint8_t& pCbcId);

    	public:
    	    std::vector<uint32_t> fVecReq;

        public:
            static const std::string fStrI2cSettings;
			static const std::string fStrI2cCommand;
			static const std::string fStrI2cReply;
			static const uint32_t fI2cSlave;

        public:
            CbcInterface(const char *puHalConfigFileName);
            ~CbcInterface();

            void ConfigureCbc();
            //Not complete/tested functions
            //void ReadCbc();
            //void UpdateCbcReg( std::string pReg, UInt_t psetValue );
            //void UpdateCbcRead(Glib& pCBC,const std::string& pRegNode,const uint32_t& pVal)
            //void WriteBroadcast();

    };
}

#endif
