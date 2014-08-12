/*!
*
* \file CBCInterface.cc
* \brief CBCInterface.cc User interface for the cbc's
* \author Lorenzo BIDEGAIN, Nicolas Pierre
* \date 30/07/14
*
* Support : mail to : lorenzo.bidegain@cern.ch, nico.pierre@icloud.com
*
*/

#include "CbcInterface.h"

<<<<<<< HEAD
#define DEV_FLAG                0
#define VERIFICATION_LOOP       0
=======

#define DEV_FLAG 0
#define VERIFICATION_LOOP 0
>>>>>>> origin/Dev

namespace Ph2_HwInterface
{

	CbcInterface::CbcInterface(BeBoardFWMap& pBoardMap):
		fBoardMap(pBoardMap)
	{
		fBoardFW=NULL;
		prevBoardId=255;
	}

	CbcInterface::~CbcInterface()
	{

	}

<<<<<<< HEAD

    void CbcInterface::SelectSRAM(uint32_t pFe)
    {
        fStrSram  = (pFe ? SRAM2 : SRAM1);
        fStrOtherSram = (pFe ? SRAM1 : SRAM2);
        fStrSramUserLogic =  (pFe ? SRAM2_USR_LOGIC : SRAM1_USR_LOGIC);
        fStrOtherSramUserLogic =  (pFe ? SRAM2_USR_LOGIC : SRAM1_USR_LOGIC);
    }


    bool CbcInterface::I2cCmdAckWait( uint32_t pAckVal, uint8_t pNcount )
    {
        unsigned int cWait(100);

        if( pAckVal )
        {
			cWait = pNcount * 500;
=======
	void CbcInterface::setBoard(uint8_t pBoardId)
	{
		if(prevBoardId!=pBoardId)
		{
			BeBoardFWMap::iterator i;
			i=fBoardMap.find(pBoardId);
			if (i==fBoardMap.end())
			{
				std::cout<<"The Board: "<<pBoardId<<"doesn't exist"<<std::endl;
			}
			else
			{
				fBoardFW=i->second;
				prevBoardId=pBoardId;
			}
>>>>>>> origin/Dev
		}
	}


	void CbcInterface::ConfigureCbc(Cbc* pCbc)
	{
		setBoard(pCbc->getBeId());

		#ifdef __CBCDAQ_DEV__
		static long min(0), sec(0);
		struct timeval start0, end;
		long seconds(0), useconds(0);

<<<<<<< HEAD
			if (cVal!=pAckVal)
            {
				if(VERIFICATION_LOOP)
                    std::cout << "Waiting for the I2c command acknowledge to be " << pAckVal << " for " << uint32_t(pNcount) << " registers." << std::endl;
				usleep( cWait );
=======
		if(  DEV_FLAG )
			{
			gettimeofday(&start0, 0);
>>>>>>> origin/Dev
			}
		#endif

		std::vector<uint32_t> cVecReq;
		CbcRegMap cCbcRegMap = pCbc->getRegMap();

		for(CbcRegMap::iterator cIt = cCbcRegMap.begin(); cIt != cCbcRegMap.end(); cIt++)
		{
			EncodeReg(cIt->second,pCbc->fCbcId,cVecReq);
		}

		fBoardFW->WriteCbcBlockReg(pCbc->fFeId,cVecReq);

		if(VERIFICATION_LOOP)
		{
			std::vector<uint32_t> cWriteValue, cReadValue, cVecReqBis;
			uint8_t cCbcId = pCbc->getCbcId();
			CbcRegItem cRegItem;

<<<<<<< HEAD
        WriteReg(PC_CONFIG_OK,0);

        WriteReg(fStrSramUserLogic,1);
        WriteReg(fStrOtherSramUserLogic,1);

    	pVecReq.push_back(0xFFFFFFFF);

    	WriteReg(fStrOtherSramUserLogic,0);

        WriteReg(fStrOtherSram,0xFFFFFFFF);

        WriteReg(fStrOtherSramUserLogic,1);

        WriteReg(fStrSramUserLogic,0);

    	WriteBlockReg(fStrSram,pVecReq);

    	WriteReg(fStrSramUserLogic,1);

        //WriteReg(CBC_HARD_RESET,0);
=======
			myflush( std::cin );

			for(int32_t i=0;i<cVecReq.size();i++)
			{
				DecodeReg(cRegItem,cCbcId,cVecReq[i]);

				cWriteValue.push_back(cRegItem.fValue);
				cRegItem.fValue = 0;

				EncodeReg(cRegItem,pCbc->fCbcId,cVecReqBis);

				fBoardFW->ReadCbcBlockReg(pCbc->fFeId,cVecReqBis);

				DecodeReg(cRegItem,cCbcId,cVecReqBis[0]);
>>>>>>> origin/Dev

				cReadValue.push_back(cRegItem.fValue);

				cVecReqBis.clear();

				if(cReadValue[i] != cWriteValue[i])
				{
					std::cout << "\nERROR !!!\nValues are not coinciding :\n" << "Written Value : " << cWriteValue[i] << "\nReadback Value : " << cReadValue[i] << std::endl;
					std::cout << "Register Adress : " << uint32_t(cRegItem.fAddress) << std::endl;
					mypause();
				}
				else
				{
					std::cout << "Writing correctly done :\n" << "Written Value : " << cWriteValue[i] << "\nReadback Value : " << cReadValue[i] << std::endl;
				}
			}

		}

		#ifdef __CBCDAQ_DEV__
		if(  DEV_FLAG )
			{
			gettimeofday( &end, 0 );
			seconds = end.tv_sec - start0.tv_sec;
			useconds = end.tv_usec - start0.tv_usec;
			min += ( seconds + useconds / 1000000 ) /60;
			sec += ( seconds + useconds / 1000000 ) %60;
			std::cout << "Time took for Cbc register configuration so far = " << min << " min " << sec << " sec." << std::endl;
			}
		#endif

	}


	void CbcInterface::WriteCbcReg(Cbc* pCbc, const std::string& pRegNode, uint32_t pValue)
	{

#ifdef __CBCDAQ_DEV__
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

        setBoard(pCbc->getBeId());

        EncodeReg(cRegItem,pCbc->fCbcId,cVecReq);

        fBoardFW->WriteCbcBlockReg(pCbc->fFeId,cVecReq);

        if(VERIFICATION_LOOP)
        {
            uint32_t cWriteValue, cReadValue;
            uint8_t cCbcId = pCbc->getCbcId();

            DecodeReg(cRegItem,cCbcId,cVecReq[0]);

            cWriteValue = cRegItem.fValue;
            cRegItem.fValue = 0;

            EncodeReg(cRegItem,pCbc->fCbcId,cVecReq);

            fBoardFW->ReadCbcBlockReg(pCbc->fFeId,cVecReq);

            DecodeReg(cRegItem,cCbcId,cVecReq[0]);

            cReadValue = cRegItem.fValue;

            if(cReadValue != cWriteValue)
            {
                std::cout << "ERROR !!!\nValues are not coinciding :\n" << "Written Value : " << cWriteValue << "\nReadback Value : " << cReadValue << std::endl;
            }
            else
            {
                std::cout << "Writing correctly done :\n" << "Written Value : " << cWriteValue << "\nReadback Value : " << cReadValue << std::endl;
            }
		}

		pCbc->setReg(pRegNode,cRegItem.fValue);

#ifdef __CBCDAQ_DEV__
		if(  DEV_FLAG )
		{
			gettimeofday( &end, 0 );
			seconds = end.tv_sec - start0.tv_sec;
			useconds = end.tv_usec - start0.tv_usec;
			min += ( seconds + useconds / 1000000 ) /60;
			sec += ( seconds + useconds / 1000000 ) %60;
			std::cout << "Time took for Cbc register update so far = " << min << " min " << sec << " sec." << std::endl;
		}
#endif


	}


	void CbcInterface::WriteCbcMultReg(Cbc* pCbc, std::vector< std::pair<std::string,uint32_t> > pVecReq)
	{

#ifdef __CBCDAQ_DEV__
		static long min(0), sec(0);
		struct timeval start0, end;
		long seconds(0), useconds(0);

		if(  DEV_FLAG ){
			gettimeofday(&start0, 0);
		}
#endif

<<<<<<< HEAD
        SelectSRAM(0);
		EnableI2c(pCbc,1);
=======
		setBoard(pCbc->getBeId());

		std::vector<uint32_t> cVecReq;
		CbcRegItem cRegItem;
>>>>>>> origin/Dev

		for(uint32_t i=0; i<pVecReq.size(); i++)
		{
			cRegItem.fValue = pVecReq[i].second;
			cRegItem = (pCbc->getRegMap())[pVecReq[i].first];

			EncodeReg(cRegItem,pCbc->fCbcId,cVecReq);

			pCbc->setReg(pVecReq[i].first,cRegItem.fValue);
		}

		fBoardFW->WriteCbcBlockReg(pCbc->fFeId,cVecReq);

		if(VERIFICATION_LOOP)
		{
			std::vector<uint32_t> cWriteValue, cReadValue, cVecReqBis;
			uint8_t cCbcId = pCbc->getCbcId();
			CbcRegItem cRegItem;

			myflush( std::cin );

			for(int32_t i=0;i<cVecReq.size();i++)
			{
				DecodeReg(cRegItem,cCbcId,cVecReq[i]);

				cWriteValue.push_back(cRegItem.fValue);
				cRegItem.fValue = 0;

				EncodeReg(cRegItem,pCbc->fCbcId,cVecReqBis);

				fBoardFW->ReadCbcBlockReg(pCbc->fFeId,cVecReq);

				DecodeReg(cRegItem,cCbcId,cVecReqBis[0]);

				cReadValue.push_back(cRegItem.fValue);

				cVecReqBis.clear();

				if(cReadValue[i] != cWriteValue[i])
				{
					std::cout << "\nERROR !!!\nValues are not coinciding :\n" << "Written Value : " << cWriteValue[i] << "\nReadback Value : " << cReadValue[i] << std::endl;
					std::cout << "Register Adress : " << uint32_t(cRegItem.fAddress) << std::endl;
					mypause();
				}
				else
				{
					std::cout << "Writing correctly done :\n" << "Written Value : " << cWriteValue[i] << "\nReadback Value : " << cReadValue[i] << std::endl;
				}
			}

		}

#ifdef __CBCDAQ_DEV__
		if(  DEV_FLAG )
		{
			gettimeofday( &end, 0 );
			seconds = end.tv_sec - start0.tv_sec;
			useconds = end.tv_usec - start0.tv_usec;
			min += ( seconds + useconds / 1000000 ) /60;
			sec += ( seconds + useconds / 1000000 ) %60;
			std::cout << "Time took for Cbc register update so far = " << min << " min " << sec << " sec." << std::endl;
		}
#endif


	}


	void CbcInterface::ReadCbcReg(Cbc* pCbc,const std::string& pRegNode)
	{

#ifdef __CBCDAQ_DEV__
		static long min(0), sec(0);
		struct timeval start0, end;
		long seconds(0), useconds(0);

		if(  DEV_FLAG ){
			gettimeofday(&start0, 0);
		}
#endif

<<<<<<< HEAD
        SelectSRAM(0);
		EnableI2c(pCbc,1);
=======
		uint8_t cCbcId;
		CbcRegItem cRegItem = (pCbc->getRegMap())[pRegNode];
		std::vector<uint32_t> cVecReq;
>>>>>>> origin/Dev

		setBoard(pCbc->getBeId());

		EncodeReg(cRegItem,pCbc->fCbcId,cVecReq);

		fBoardFW->ReadCbcBlockReg(pCbc->fFeId,cVecReq);

		DecodeReg(cRegItem,cCbcId,cVecReq[0]);

#ifdef __CBCDAQ_DEV__
        std::cout << "CbcId : " << uint32_t(cCbcId) << std::endl;
        std::cout << "Value read : " << uint32_t(cRegItem.fValue) << std::endl;
#endif

		pCbc->setReg(pRegNode,cRegItem.fValue);

#ifdef __CBCDAQ_DEV__
		if(  DEV_FLAG ){
			gettimeofday( &end, 0 );
			seconds = end.tv_sec - start0.tv_sec;
			useconds = end.tv_usec - start0.tv_usec;
			min += ( seconds + useconds / 1000000 ) /60;
			sec += ( seconds + useconds / 1000000 ) %60;
			std::cout << "Time took for Cbc register refresh so far = " << min << " min " << sec << " sec." << std::endl;
		}
#endif


<<<<<<< HEAD
	void CbcInterface::EncodeReg(CbcRegItem& pRegItem, uint8_t& pCbcId, std::vector<uint32_t>& pVecReq)
	{
		pVecReq.push_back(pCbcId<<17 | pRegItem.fPage<<16 | pRegItem.fAddress<<8 | pRegItem.fValue);
=======
>>>>>>> origin/Dev
	}


	void CbcInterface::ReadCbcMultReg(Cbc* pCbc,std::vector<std::string> pVecReg)
	{
<<<<<<< HEAD
		uint32_t cMask(0x00000000);
		unsigned int i(0);

		for( i=17; i < 18; i++ ) cMask |= ( (uint32_t) 1 << i );
		pCbcId = ( ( pWord & cMask ) >> 17 );

		for( cMask=0, i=16; i < 17; i++ ) cMask |= (uint32_t) 1 << i;
		pRegItem.fPage = ( pWord & cMask ) >> 16;

		for( cMask=0, i=8; i < 16; i++ ) cMask |= (uint32_t) 1 << i;
		pRegItem.fAddress = ( pWord & cMask ) >> 8;

		for( cMask=0, i=0; i < 8; i++ ) cMask |= (uint32_t) 1 << i;
		pRegItem.fValue = pWord & cMask;
	}

=======
>>>>>>> origin/Dev

#ifdef __CBCDAQ_DEV__
		static long min(0), sec(0);
		struct timeval start0, end;
		long seconds(0), useconds(0);

		if(  DEV_FLAG ){
			gettimeofday(&start0, 0);
		}
#endif

		uint8_t cCbcId;
		CbcRegItem cRegItem;
		std::vector<uint32_t> cVecReq;
<<<<<<< HEAD
		CbcRegMap cCbcRegMap = pCbc->getRegMap();
=======
>>>>>>> origin/Dev

		setBoard(pCbc->getBeId());

		for(uint32_t i=0; i<pVecReg.size(); i++)
		{
			cRegItem = (pCbc->getRegMap())[pVecReg[i]];

			EncodeReg(cRegItem,pCbc->fCbcId,cVecReq);

			fBoardFW->ReadCbcBlockReg(pCbc->fFeId,cVecReq);

			DecodeReg(cRegItem,cCbcId,cVecReq[0]);

#ifdef __CBCDAQ_DEV__
			std::cout << "CbcId : " << uint32_t(cCbcId) << std::endl;
			std::cout << "Value read : " << uint32_t(cRegItem.fValue) << std::endl;
#endif

			pCbc->setReg(pVecReg[i],cRegItem.fValue);
		}

#ifdef __CBCDAQ_DEV__
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


<<<<<<< HEAD
	void CbcInterface::UpdateAllCbc(Module* pModule)
=======
	void CbcInterface::ReadAllCbc(Module* pModule)
>>>>>>> origin/Dev
	{

#ifdef __CBCDAQ_DEV__
		static long min(0), sec(0);
		struct timeval start0, end;
		long seconds(0), useconds(0);

		if(  DEV_FLAG ){
			gettimeofday(&start0, 0);
		}
#endif
        CbcRegItem cRegItem;
        uint8_t cCbcId;
        std::vector<uint32_t> cVecReq;
        std::vector<std::string> cVecRegNode;
        Cbc *cCbc;
        int cMissed=0;
<<<<<<< HEAD
=======

		setBoard(pModule->getBeId());
>>>>>>> origin/Dev

		for(uint8_t i=0;i<pModule->getNCbc();i++)
		{

            if(pModule->getCbc(i+cMissed) == NULL)
            {
                i--;
                cMissed++;
            }

            else
            {
<<<<<<< HEAD
=======

>>>>>>> origin/Dev
                cCbc = pModule->getCbc(i+cMissed);

                CbcRegMap cCbcRegMap = cCbc->getRegMap();

                for(CbcRegMap::iterator cIt = cCbcRegMap.begin(); cIt != cCbcRegMap.end(); cIt++)
                {
                   EncodeReg(cIt->second,cCbc->fCbcId,cVecReq);
                   cVecRegNode.push_back(cIt->first);
                }

<<<<<<< HEAD
                ReadCbcBlockReg(cCbc,cVecReq);
=======
                fBoardFW->ReadCbcBlockReg(cCbc->fFeId,cVecReq);
>>>>>>> origin/Dev

                for(uint32_t j=0;j<cVecReq.size();j++)
                {
                    DecodeReg(cRegItem,cCbcId,cVecReq[j]);

#ifdef __CBCDAQ_DEV__
                    std::cout << "CbcId : " << uint32_t(cCbcId) << std::endl;
                    std::cout << "Value read : " << uint32_t(cRegItem.fValue) << std::endl;
#endif

                    cCbc->setReg(cVecRegNode[j],cRegItem.fValue);
                }
            }
		}

#ifdef __CBCDAQ_DEV__
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


	void CbcInterface::WriteBroadcast(Module* pModule,const std::string& pRegNode, uint32_t pValue)
	{
		setBoard(pModule->getBeId());

		#ifdef __CBCDAQ_DEV__
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

		for(uint8_t i=0;i<pModule->getNCbc();i++)
		{
			if(pModule->getCbc(i+cMissed) == NULL)
			{
				i--;
				cMissed++;
			}

            	/*
                It makes the broadcast only the first time it finds an
                existing Cbc and then update all the other Cbcs.
            	*/
			else if(i == 0 && pModule->getCbc(i+cMissed) != NULL)
			{
				cCbc = pModule->getCbc(i+cMissed);
				CbcRegItem cRegItem = (cCbc->getRegMap())[pRegNode];
				cRegItem.fValue = pValue;

				EncodeReg(cRegItem,cCbcId,cVecReq);

				fBoardFW->WriteCbcBlockReg(cCbc->fFeId,cVecReq);

<<<<<<< HEAD
				UpdateCbc(pModule->getCbc(i+cMissed),pRegNode);
=======
				ReadCbcReg(pModule->getCbc(i+cMissed),pRegNode);
>>>>>>> origin/Dev
			}

			else
			{
<<<<<<< HEAD
				UpdateCbc(pModule->getCbc(i+cMissed),pRegNode);
=======
				ReadCbcReg(pModule->getCbc(i+cMissed),pRegNode);
>>>>>>> origin/Dev
			}
		}

		#ifdef __CBCDAQ_DEV__
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

<<<<<<< HEAD

	void CbcInterface::WriteCbc(Cbc* pCbc, const std::string& pRegNode, uint32_t pValue)
	{
#ifdef __CBCDAQ_DEV__
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

        if(VERIFICATION_LOOP)
        {
            uint32_t cWriteValue, cReadValue;
            uint8_t cCbcId = pCbc->getCbcId();

            DecodeReg(cRegItem,cCbcId,cVecReq[0]);

            cWriteValue = cRegItem.fValue;
            cRegItem.fValue = 0;

            EncodeReg(cRegItem,pCbc->fCbcId,cVecReq);

            ReadCbcBlockReg(pCbc,cVecReq);

            DecodeReg(cRegItem,cCbcId,cVecReq[0]);

            cReadValue = cRegItem.fValue;

            if(cReadValue != cWriteValue)
            {
                std::cout << "ERROR !!!\nValues are not coinciding :\n" << "Written Value : " << cWriteValue << "\nReadback Value : " << cReadValue << std::endl;
            }
            else
            {
                std::cout << "Writing correctly done :\n" << "Written Value : " << cWriteValue << "\nReadback Value : " << cReadValue << std::endl;
            }

        }

#ifdef __CBCDAQ_DEV__
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


	void CbcInterface::UpdateCbc(Cbc* pCbc,const std::string& pRegNode)
	{
#ifdef __CBCDAQ_DEV__
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

#ifdef __CBCDAQ_DEV__
        std::cout << "CbcId : " << uint32_t(cCbcId) << std::endl;
        std::cout << "Value read : " << uint32_t(cRegItem.fValue) << std::endl;
#endif
=======
	void CbcInterface::CbcHardReset(Cbc* pCbc)
	{
		setBoard(pCbc->getBeId());

		fBoardFW->WriteReg(CBC_HARD_RESET,1);

		usleep(200000);
>>>>>>> origin/Dev

		fBoardFW->WriteReg(CBC_HARD_RESET,0);

		usleep(200000);
	}

	void CbcInterface::CbcFastReset(Cbc* pCbc)
	{
		setBoard(pCbc->getBeId());

		fBoardFW->WriteReg(CBC_FAST_RESET,1);

		usleep(200000);

		fBoardFW->WriteReg(CBC_FAST_RESET,0);
	}


	void CbcInterface::EncodeReg(CbcRegItem& pRegItem, uint8_t& pCbcId, std::vector<uint32_t>& pVecReq)
	{
		fBoardFW->EncodeReg(pRegItem, pCbcId, pVecReq);
	}


	void CbcInterface::DecodeReg(CbcRegItem& pRegItem, uint8_t& pCbcId, uint32_t pWord)
	{
		fBoardFW->DecodeReg(pRegItem, pCbcId, pWord);
	}

}
