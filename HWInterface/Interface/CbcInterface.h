/*

  FileName : 					CbcInterface.h
  Content : 					 CbcInterface class, config of the Cbcs
  Programmer : 				  Nicolas PIERRE
  Version : 					 0.0
  Date of creation : 	        07/06/14
  Support : 					 mail to : nicolas.pierre@etu.unistra.net

*/

#ifndef __CBCINTERFACE_H__
#define __CBCINTERFACE_H__
#include <time.h>
#include <string>
#include <TROOT.h>
#include <limits.h>
#include <boost/cstdint.hpp>
#include "RegManager.h"

namespace Ph2_HwInterface
{
    class CBCInterface : public RegManager
    {
        private:
            std::string fStrSram, fStrOtherSram, fStrSramUserLogic, fStrFull, fStrReadout;
            //Cbc fCbc;

        public:
            static const std::string fStrI2cSettings;
			static const std::string fStrI2cCommand;
			static const std::string fStrI2cReply;
			static const uint32_t fI2cSlave;

        public:
            CBCInterface();
            ~CBCInterface();

			void SelectSramForI2C( unsigned int pFe );
			bool I2cCmdAckWait( uint32_t pAckVal, unsigned int pNcount=1 );
			void SendBlockCbcI2cRequest( uint32_t pFe, std::vector<uint32_t>& pVecReq, bool pWrite);
			void ReadI2cValuesInSRAM( unsigned int pFe, std::vector<uint32_t> &pVecReq );

            void EnableI2c( bool pEnable );//not tested
			uint32_t SendI2c( uint16_t pAddr, uint16_t pData, bool pWrite );//not tested
    };
}

#endif
