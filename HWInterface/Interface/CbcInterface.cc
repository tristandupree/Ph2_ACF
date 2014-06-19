/*

  FileName : 					CbcInterface.cpp
  Content : 					 CbcInterface class, config of the Cbcs
  Programmer : 				  Nicolas PIERRE
  Version : 					 1.3
  Date of creation : 	        07/06/14
  Support : 					 mail to : nicolas.pierre@cern.ch

*/

#include <uhal/uhal.hpp>
#include <boost/format.hpp>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <time.h>
#include "CbcInterface.h"
#include "Exception.h"

#define I2C_CTRL_ENABLE 		0x000009F4
#define I2C_CTRL_DISABLE		0
#define I2C_STROBE			  1
#define I2C_M16B				0
#define I2C_MEM				 1

#define I2C_SLAVE			   0x5B
#define I2C_COMMAND			 "user_wb_ttc_fmc_regs.Cbc_reg_i2c_command"
#define I2C_REPLY			   "user_wb_ttc_fmc_regs.Cbc_reg_i2c_reply"
#define I2C_SETTINGS            "user_wb_ttc_fmc_regs.Cbc_reg_i2c_settings"

#define MAX_NB_LOOP			 50

#define DEV_FLAG                0

namespace Ph2_HwInterface
{
	const std::string CbcInterface::fStrI2cCommand = I2C_COMMAND;
	const std::string CbcInterface::fStrI2cReply = I2C_REPLY;
	const uint32_t CbcInterface::fI2cSlave = I2C_SLAVE;
    const std::string CbcInterface::fStrI2cSettings = I2C_SETTINGS;


    CbcInterface::CbcInterface(const char *puHalConfigFileName):
		RegManager(puHalConfigFileName)
	{

	}


	CbcInterface::~CbcInterface()
	{

	}


	void CbcInterface::SelectSramForI2C( uint8_t pCbcId )
    {
        fStrSram = (pCbcId==0 ? "sram1" : "sram2");
		fStrOtherSram = (pCbcId==0 ? "sram2" : "sram1");
		fStrSramUserLogic = (pCbcId==0 ? "ctrl_sram.sram1_user_logic" : "ctrl_sram.sram2_user_logic");
    }


    bool CbcInterface::I2cCmdAckWait( uint32_t pAckVal, uint8_t pNcount )
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
			cVal=ReadReg("Cbc_i2c_cmd_ack");

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


    void CbcInterface::SendBlockCbcI2cRequest( uint32_t pCbcId, std::vector<uint32_t>& pVecReq, bool pWrite)
    {
		SelectSramForI2C( pCbcId );

		WriteReg(fStrSramUserLogic,1);

		pVecReq.push_back(0xFFFFFFFF);

		WriteReg(fStrSramUserLogic,0);

		WriteBlockReg(fStrSram,pVecReq);
		WriteReg(fStrOtherSram,0xFFFFFFFF);

		WriteReg(fStrSramUserLogic,1);

        //r/w request
		WriteReg("Cbc_i2c_cmd_rq",pWrite ? 3: 1);

		pVecReq.pop_back();

		if( I2cCmdAckWait( (uint32_t)1, pVecReq.size() ) ==0 )
        {
			throw Exception( Form( "%s: I2cCmdAckWait %d failed.", "CbcInterface", 1 ) );
		}

		WriteReg("Cbc_i2c_cmd_rq",0);

		if( I2cCmdAckWait( (uint32_t)0, pVecReq.size() ) ==0 )
        {
			throw Exception( Form( "%s: I2cCmdAckWait %d failed.", "CbcInterface", 0 ) );
		}
	}


    void CbcInterface::ReadI2cBlockValuesInSRAM( unsigned int pCbcId, std::vector<uint32_t> &pVecReq )
    {
		SelectSramForI2C( pCbcId );

		WriteReg(fStrSramUserLogic,0);

		uhal::ValVector<uint32_t> cData = ReadBlockReg(fStrSram,pVecReq.size());

		WriteReg(fStrSramUserLogic,1);
		WriteReg("Cbc_i2c_cmd_rq",0);

		std::vector<uint32_t>::iterator it = pVecReq.begin();
		uhal::ValVector< uint32_t >::const_iterator itValue = cData.begin();

        while( it != pVecReq.end() )
        {
			*it = *itValue;
			it++; itValue++;
		}
    }


    void CbcInterface::EnableI2c( Cbc& pCbc, bool pEnable )
    {
		ChooseBoard(pCbc.fBeId);

		uint32_t cValue = I2C_CTRL_ENABLE;

        if( !pEnable )
            cValue = I2C_CTRL_DISABLE;

		WriteReg(fStrI2cSettings,cValue);

        if( pEnable )
            usleep(100000);
	}


	void CbcInterface::WriteCbcBlockReg( Cbc& pCbc, std::vector<uint32_t>& pVecReq )
	{
#ifdef __CbcDAQ_DEV__
		static long min(0), sec(0);
		struct timeval start0, end;
		long seconds(0), useconds(0);

		if(  DEV_FLAG ){
			gettimeofday(&start0, 0);
		}
#endif

		ChooseBoard(pCbc.fBeId);

		try
		{
			SendBlockCbcI2cRequest( pCbc.fCbcId, pVecReq, true );
		}

		catch( Exception &except )
		{
			throw except;
		}

#ifdef __CbcDAQ_DEV__
		if( DEV_FLAG ){
			gettimeofday( &end, 0 );
			seconds = end.tv_sec - start0.tv_sec;
			useconds = end.tv_usec - start0.tv_usec;
			min += ( seconds + useconds / 1000000 ) /60;
			sec += ( seconds + useconds / 1000000 ) %60;
			std::cout << "Time took for Cbc register write so far = " << min << " min " << sec << " sec." << std::endl;
		}
#endif
	}


	void CbcInterface::ReadCbcBlockReg( Cbc& pCbc, std::vector<uint32_t>& pVecReq )
	{

#ifdef __CbcDAQ_DEV__
		static long min(0), sec(0);
		struct timeval start0, end;
		long seconds(0), useconds(0);

		if(  DEV_FLAG ){
			gettimeofday(&start0, 0);
		}
#endif

		ChooseBoard(pCbc.fBeId);

		try
		{
			SendBlockCbcI2cRequest( pCbc.fCbcId, pVecReq, false);
		}

		catch( Exception &e )
		{
			throw e;
		}

#ifdef __CbcDAQ_DEV__
		if(  DEV_FLAG ){
			gettimeofday( &end, 0 );
			seconds = end.tv_sec - start0.tv_sec;
			useconds = end.tv_usec - start0.tv_usec;
			min += ( seconds + useconds / 1000000 ) /60;
			sec += ( seconds + useconds / 1000000 ) %60;
			std::cout << "Time took for Cbc register read so far = " << min << " min " << sec << " sec." << std::endl;
		}
#endif

		ReadI2cBlockValuesInSRAM( pCbc.fCbcId, pVecReq );

	}


	void CbcInterface::EncodeReg(CbcRegItem& pRegItem, uint8_t pCbcId)
	{
		fVecReq.push_back(pCbcId<<24 | pRegItem.fPage<<16 | pRegItem.fAddress<<8 | pRegItem.fValue);
	}


	void CbcInterface::DecodeReg(uint32_t pWord, CbcRegItem& pRegItem, uint8_t& pCbcId)
	{
		uint32_t cMask(0x00000000);
		unsigned int i(0);

		for( i=24; i < 32; i++ ) cMask |= ( (uint32_t) 1 << i );
		pCbcId = ( ( pWord & cMask ) >> 24 );

		for( cMask=0, i=16; i < 24; i++ ) cMask |= (uint32_t) 1 << i;
		pRegItem.fPage = ( pWord & cMask ) >> 16;

		for( cMask=0, i=8; i < 16; i++ ) cMask |= (uint32_t) 1 << i;
		pRegItem.fAddress = ( pWord & cMask ) >> 8;

		for( cMask=0, i=0; i < 8; i++ ) cMask |= (uint32_t) 1 << i;
		pRegItem.fValue = pWord & cMask;
	}


	void CbcInterface::ConfigureCbc()
	{

	}

/*
	void CbcInterface::UpdateCbcWrite( uint8_t pCbcId, std::string pCbcId, uint8_t psetValue )
	{
		ChooseBoard(pGlib.getBeId());

		WriteCbcBlockReg(uint16_t pCbcId, std::vector<uint32_t>& pVecReq);
		Cbc.setReg(std::string pCbcId, uint8_t psetValue);
	}

	void CbcInterface::UpdateCbcRead(Glib& pCBC,const std::string& pRegNode,const uint32_t& pVal)
	{
		ChooseBoard(pGlib.getBeId());

		pGlib.setReg(pRegNode,(uint32_t) ReadCbcBlockReg(pRegNode));
	}
*/
}
