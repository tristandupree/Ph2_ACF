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
#include "../HWDescription/Definition.h"
#include "../HWDescription/Module.h"
#include "../HWDescription/Cbc.h"

#define DEV_FLAG                0

namespace Ph2_HwInterface
{

    CbcInterface::CbcInterface(const char *puHalConfigFileName):
		RegManager(puHalConfigFileName)
	{

	}


	CbcInterface::~CbcInterface()
	{

	}


	void CbcInterface::SelectSramForI2C( uint8_t pCbcId )
    {
        fStrSram = (pCbcId==0 ? SRAM1 : SRAM2);
		fStrOtherSram = (pCbcId==0 ? SRAM2 : SRAM1);
		fStrSramUserLogic = (pCbcId==0 ? SRAM1_USR_LOGIC : SRAM2_USR_LOGIC);
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
			cVal=ReadReg(CBC_I2C_CMD_ACK);

			if (cVal!=pAckVal)
            {
				std::cout << "Waiting for the I2c command acknowledge to be " << pAckVal << " for " << uint32_t(pNcount) << " registers." << std::endl;
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
		//WriteReg(fStrOtherSram,0xFFFFFFFF);

		WriteReg(fStrSramUserLogic,1);

        //r/w request
		WriteReg(CBC_I2C_CMD_RQ,pWrite ? 3: 1);

		pVecReq.pop_back();

		if( I2cCmdAckWait( (uint32_t)1, pVecReq.size() ) ==0 )
        {
			throw Exception( Form( "%s: I2cCmdAckWait %d failed.", "CbcInterface", 1 ) );
		}

		WriteReg(CBC_I2C_CMD_RQ,0);

		if( I2cCmdAckWait( (uint32_t)0, pVecReq.size() ) ==0 )
        {
			throw Exception( Form( "%s: I2cCmdAckWait %d failed.", "CbcInterface", 0 ) );
		}
	}


    void CbcInterface::ReadI2cBlockValuesInSRAM( unsigned int pCbcId, std::vector<uint32_t> &pVecReq )
    {
		SelectSramForI2C( pCbcId );

		WriteReg(fStrSramUserLogic,0);

		uhal::ValVector<uint32_t> cData = ReadBlockReg(fStrSram,pVecReq.size()+1);

		WriteReg(fStrSramUserLogic,1);
		WriteReg(CBC_I2C_CMD_RQ,0);

		std::vector<uint32_t>::iterator it = pVecReq.begin();
		uhal::ValVector< uint32_t >::const_iterator itValue = cData.begin();

        while( it != pVecReq.end() )
        {
			*it = *itValue;
			it++; itValue++;
		}
    }


    void CbcInterface::EnableI2c( Cbc* pCbc, bool pEnable )
    {
		ChooseBoard(pCbc->fBeId);

		uint32_t cValue = I2C_CTRL_ENABLE;

        if( !pEnable )
            cValue = I2C_CTRL_DISABLE;

		WriteReg(I2C_SETTINGS,cValue);

        if( pEnable )
            usleep(100000);
	}


	void CbcInterface::WriteCbcBlockReg( Cbc* pCbc, std::vector<uint32_t>& pVecReq )
	{
#ifdef __CbcDAQ_DEV__
		static long min(0), sec(0);
		struct timeval start0, end;
		long seconds(0), useconds(0);

		if(  DEV_FLAG ){
			gettimeofday(&start0, 0);
		}
#endif

		EnableI2c(pCbc,1);

		try
		{
			SendBlockCbcI2cRequest( pCbc->fCbcId, pVecReq, true );
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

		EnableI2c(pCbc,0);

		}
#endif
	}


	void CbcInterface::ReadCbcBlockReg( Cbc* pCbc, std::vector<uint32_t>& pVecReq )
	{

#ifdef __CbcDAQ_DEV__
		static long min(0), sec(0);
		struct timeval start0, end;
		long seconds(0), useconds(0);

		if(  DEV_FLAG ){
			gettimeofday(&start0, 0);
		}
#endif

		EnableI2c(pCbc,1);

		try
		{
			SendBlockCbcI2cRequest( pCbc->fCbcId, pVecReq, false);
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

		ReadI2cBlockValuesInSRAM( pCbc->fCbcId, pVecReq );

		EnableI2c(pCbc,0);

	}


	void CbcInterface::EncodeReg(CbcRegItem& pRegItem, uint8_t& pCbcId, std::vector<uint32_t>& pVecReq)
	{
		pVecReq.push_back(pCbcId<<24 | pRegItem.fPage<<16 | pRegItem.fAddress<<8 | pRegItem.fValue);
	}


	void CbcInterface::DecodeReg(CbcRegItem& pRegItem, uint8_t& pCbcId, uint32_t pWord)
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


	void CbcInterface::ConfigureCbc(Cbc* pCbc)
	{
#ifdef __CbcDAQ_DEV__
		static long min(0), sec(0);
		struct timeval start0, end;
		long seconds(0), useconds(0);

		if(  DEV_FLAG ){
			gettimeofday(&start0, 0);
		}
#endif

		std::vector<uint32_t> cVecReq;
		CbcRegMap cCbcRegMap = pCbc->getRegMap();

		for(CbcRegMap::iterator cIt = cCbcRegMap.begin(); cIt != cCbcRegMap.end(); cIt++)
		{
			EncodeReg(cIt->second,pCbc->fCbcId,cVecReq);
		}

		WriteCbcBlockReg(pCbc,cVecReq);

#ifdef __CbcDAQ_DEV__
		if(  DEV_FLAG ){
			gettimeofday( &end, 0 );
			seconds = end.tv_sec - start0.tv_sec;
			useconds = end.tv_usec - start0.tv_usec;
			min += ( seconds + useconds / 1000000 ) /60;
			sec += ( seconds + useconds / 1000000 ) %60;
			std::cout << "Time took for Cbc register configuration so far = " << min << " min " << sec << " sec." << std::endl;
		}
#endif

	}


	void CbcInterface::ReadCbc(Module* pModule,const std::string& pRegNode)
	{

#ifdef __CbcDAQ_DEV__
		static long min(0), sec(0);
		struct timeval start0, end;
		long seconds(0), useconds(0);

		if(  DEV_FLAG ){
			gettimeofday(&start0, 0);
		}
#endif

		for(uint8_t i=0;i<pModule->getNCbc();i++)
		{
			UpdateCbcRead(pModule->getCbc(i),pRegNode);
		}

#ifdef __CbcDAQ_DEV__
		if(  DEV_FLAG ){
			gettimeofday( &end, 0 );
			seconds = end.tv_sec - start0.tv_sec;
			useconds = end.tv_usec - start0.tv_usec;
			min += ( seconds + useconds / 1000000 ) /60;
			sec += ( seconds + useconds / 1000000 ) %60;
			std::cout << "Time took for reading all Cbcs so far = " << min << " min " << sec << " sec." << std::endl;
		}
#endif

	}


	void CbcInterface::WriteBroadcast(Module* pModule,const std::string& pRegNode, uint8_t pValue)
	{

#ifdef __CbcDAQ_DEV__
		static long min(0), sec(0);
		struct timeval start0, end;
		long seconds(0), useconds(0);

		if(  DEV_FLAG )
		{
			gettimeofday(&start0, 0);
		}
#endif

		uint8_t cCbcId = 0xFF;
		std::vector<uint32_t> cVecReq;
		Cbc* cCbc;
		int cMissed = 0;
		int cFirst = 1;

		for(uint8_t i=0;i<pModule->getNCbc();i++)
		{
			if(pModule->getCbc(i+cMissed) == NULL)
			{
				i--;
				cMissed++;
			}

			else if(cFirst == 1)
			{
				cCbc = pModule->getCbc(i+cMissed);
				CbcRegItem cRegItem = (cCbc->getRegMap())[pRegNode];
				cRegItem.fValue = pValue;

				cFirst = 0;

				EncodeReg(cRegItem,cCbcId,cVecReq);

				WriteCbcBlockReg(cCbc,cVecReq);

				UpdateCbcRead(pModule->getCbc(i+cMissed),pRegNode);
			}

			else
			{
				UpdateCbcRead(pModule->getCbc(i+cMissed),pRegNode);
			}
		}

#ifdef __CbcDAQ_DEV__
		if(  DEV_FLAG )
		{
			gettimeofday( &end, 0 );
			seconds = end.tv_sec - start0.tv_sec;
			useconds = end.tv_usec - start0.tv_usec;
			min += ( seconds + useconds / 1000000 ) /60;
			sec += ( seconds + useconds / 1000000 ) %60;
			std::cout << "Time took for writing all Cbcs so far = " << min << " min " << sec << " sec." << std::endl;
		}
#endif

	}


	void CbcInterface::UpdateCbcWrite(Cbc* pCbc, const std::string& pRegNode, uint8_t pValue)
	{
#ifdef __CbcDAQ_DEV__
		static long min(0), sec(0);
		struct timeval start0, end;
		long seconds(0), useconds(0);

		if(  DEV_FLAG ){
			gettimeofday(&start0, 0);
		}
#endif

		CbcRegItem cRegItem = (pCbc->getRegMap())[pRegNode];
		std::vector<uint32_t> cVecReq;

		cRegItem.fValue = pValue;

		ChooseBoard(pCbc->getBeId());

		EncodeReg(cRegItem,pCbc->fCbcId,cVecReq);

		WriteCbcBlockReg(pCbc,cVecReq);

		pCbc->setReg(pRegNode,cRegItem.fValue);

#ifdef __CbcDAQ_DEV__
		if(  DEV_FLAG ){
			gettimeofday( &end, 0 );
			seconds = end.tv_sec - start0.tv_sec;
			useconds = end.tv_usec - start0.tv_usec;
			min += ( seconds + useconds / 1000000 ) /60;
			sec += ( seconds + useconds / 1000000 ) %60;
			std::cout << "Time took for Cbc register update so far = " << min << " min " << sec << " sec." << std::endl;
		}
#endif

	}


	void CbcInterface::UpdateCbcRead(Cbc* pCbc,const std::string& pRegNode)
	{
#ifdef __CbcDAQ_DEV__
		static long min(0), sec(0);
		struct timeval start0, end;
		long seconds(0), useconds(0);

		if(  DEV_FLAG ){
			gettimeofday(&start0, 0);
		}
#endif

		uint8_t cCbcId;
		CbcRegItem cRegItem = (pCbc->getRegMap())[pRegNode];
		std::vector<uint32_t> cVecReq;

		ChooseBoard(pCbc->getBeId());

		EncodeReg(cRegItem,pCbc->fCbcId,cVecReq);

		ReadCbcBlockReg(pCbc,cVecReq);

		DecodeReg(cRegItem,cCbcId,cVecReq[0]);

		pCbc->setReg(pRegNode,cRegItem.fValue);

#ifdef __CbcDAQ_DEV__
		if(  DEV_FLAG ){
			gettimeofday( &end, 0 );
			seconds = end.tv_sec - start0.tv_sec;
			useconds = end.tv_usec - start0.tv_usec;
			min += ( seconds + useconds / 1000000 ) /60;
			sec += ( seconds + useconds / 1000000 ) %60;
			std::cout << "Time took for Cbc register refresh so far = " << min << " min " << sec << " sec." << std::endl;
		}
#endif

	}

}
