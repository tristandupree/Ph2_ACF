/*

  FileName : 					CbcInterface.cpp
  Content : 					 Cbc class, config of the Cbcs
  Programmer : 				  Nicolas PIERRE
  Version : 					 0.0
  Date of creation : 	        07/06/14
  Support : 					 mail to : nicolas.pierre@etu.unistra.net

*/

#include <uhal/uhal.hpp>
#include <boost/format.hpp>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <time.h>
#include "CBCInterface.h"

#define I2C_CTRL_ENABLE 		0x000009F4
#define I2C_CTRL_DISABLE		0

#define DEV_FLAG         0

namespace Ph2_HwInterface
{
    void CBCInterface::SelectSramForI2C( unsigned int pFe )
    {
        fStrSram = (pFe==0 ? "sram1" : "sram2");
		fStrOtherSram = (pFe==0 ? "sram2" : "sram1");
		fStrSramUserLogic = (pFe==0 ? "ctrl_sram.sram1_user_logic" : "ctrl_sram.sram2_user_logic");
    }

    bool CBCInterface::I2cCmdAckWait( uint32_t pAckVal, unsigned int pNcount=1 )
    {
        unsigned int cWait(100);

        if( pAckVal )
        {
			cWait = pNcount * 600;
		}

		usleep( cWait );

		uhal::ValWord<uint32_t> cVal;
		uint32_t cLoop=0;

        do
        {
			cVal=ReadReg("cbc_i2c_cmd_ack");

			if (cVal!=pAckVal)
            {
				std::cout << "Waiting for the I2c command acknowledge to be " << pAckVal << " for " << pNcount << " registers." << std::endl;
				usleep( cWait );
			}

		} while (cVal!= pAckVal && ++cLoop<MAX_NB_LOOP);

		if (cLoop>=MAX_NB_LOOP)
        {
			std::cout<<"Warning: time out in I2C acknowledge loop (" << pAckVal << ")" << std::endl;
			return false;
		}

		return true;
	}

    void CBCInterface::SendBlockCbcI2cRequest( uint32_t pFe, std::vector<uint32_t>& pVecReq, bool pWrite)
    {
        SelectSramForI2C( pFe );

		WriteReg(fStrSramUserLogic,1);

		pVecReq.push_back(0xFFFFFFFF);

		WriteReg(fStrSramUserLogic,0);

		WriteBlockReg(fStrSram,pVecReq);
		WriteReg(fStrOtherSram,0xFFFFFFFF);

		WriteReg(fStrSramUserLogic,1);

        //r/w request
		WriteReg("cbc_i2c_cmd_rq",pWrite ? 3: 1);

		pVecReq.pop_back();

		if( I2cCmdAckWait( (uint32_t)1, pVecReq.size() ) ==0 )
        {
			throw Exception( Form( "%s: I2cCmdAckWait %d failed.", "CBCInterface", 1 ) );
		}

		WriteReg("cbc_i2c_cmd_rq",0);

		if( I2cCmdAckWait( (uint32_t)0, pVecReq.size() ) ==0 )
        {
			throw Exception( Form( "%s: I2cCmdAckWait %d failed.", "CBCInterface", 0 ) );
		}
	}

    void CBCInterface::ReadI2cValuesInSRAM( unsigned int pFe, std::vector<uint32_t> &pVecReq )
    {
        SelectSramForI2C( pFe );

		WriteReg(fStrSramUserLogic,0);

		uhal::ValVector<uint32_t> cData = ReadBlockReg(fStrSram,pVecReq.size());

		WriteReg(fStrSramUserLogic,1);
		WriteReg("cbc_i2c_cmd_rq",0);

		std::vector<uint32_t>::iterator it = pVecReq.begin();
		uhal::ValVector< uint32_t >::const_iterator itValue = cData.begin();

        while( it != pVecReq.end() )
        {
			*it = *itValue;
			it++; itValue++;
		}
    }

    void EnableI2c( bool pEnable )
    {
		uint32_t cValue = I2C_CTRL_ENABLE;

        if( !pEnable )
            cValue = I2C_CTRL_DISABLE;

		WriteReg(fStrI2cSettings,cValue);

        if( pEnable )
            usleep(100000);
	}

    uint32_t SendI2c( uint16_t pAddr, uint16_t pData, bool pWrite )
    {
        uint32_t fmc_vtrx_comm = I2C_STROBE<<31 | I2C_M16B<<25 | I2C_MEM<<24 | (pWrite?1:0)<<23 | fI2cSlave<<16 | pAddr<<8 | pData;

        WriteReg(fStrI2cCommand,fmc_vtrx_comm);

		uhal::ValWord<uint32_t> reply = ReadReg(fStrI2cReply);

		return reply & 0xff;
    }
}
