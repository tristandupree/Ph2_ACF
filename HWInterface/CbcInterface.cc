/*

	FileName :                     CbcInterface.cc
	Content :                      User Interface to the Cbcs
	Programmer :                   Lorenzo BIDEGAIN, Nicolas PIERRE
	Version :                      1.0
	Date of creation :             10/07/14
	Support :                      mail to : lorenzo.bidegain@gmail.com, nico.pierre@icloud.com

*/

#include "CbcInterface.h"


#define DEV_FLAG 0

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
		}
	}


	void CbcInterface::ConfigureCbc(Cbc* pCbc, bool pVerifLoop)
	{
		setBoard(pCbc->getBeId());

		#ifdef __CBCDAQ_DEV__
		static long min(0), sec(0);
		struct timeval start0, end;
		long seconds(0), useconds(0);

		if(  DEV_FLAG )
			{
			gettimeofday(&start0, 0);
			}
		#endif

		std::vector<uint32_t> cVecReq;
		CbcRegMap cCbcRegMap = pCbc->getRegMap();

		for(CbcRegMap::iterator cIt = cCbcRegMap.begin(); cIt != cCbcRegMap.end(); cIt++)
		{
			EncodeReg(cIt->second,pCbc->getCbcId(),cVecReq);
		}

		fBoardFW->WriteCbcBlockReg(pCbc->getFeId(),cVecReq);

		if(pVerifLoop)
		{
			std::vector<uint32_t> cWriteValue, cReadValue, cVecReqBis;
			uint8_t cCbcId = pCbc->getCbcId();
			CbcRegItem cRegItem;

			for(int32_t i=0;i<cVecReq.size();i++)
			{
				DecodeReg(cRegItem,cCbcId,cVecReq[i]);

				cWriteValue.push_back(cRegItem.fValue);
				cRegItem.fValue = 0;

				EncodeReg(cRegItem,pCbc->getCbcId(),cVecReqBis);

				fBoardFW->ReadCbcBlockReg(pCbc->getFeId(),cVecReqBis);

				DecodeReg(cRegItem,cCbcId,cVecReqBis[0]);

				cReadValue.push_back(cRegItem.fValue);

				cVecReqBis.clear();

				if(cReadValue[i] != cWriteValue[i])
				{
					std::cout << "\nERROR !!!\nValues are not coinciding :\n" << "Written Value : " << cWriteValue[i] << "\nReadback Value : " << cReadValue[i] << std::endl;
					std::cout << "Register Adress : " << uint32_t(cRegItem.fAddress) << std::endl;
					std::cout << "Cbc Id : " << uint32_t(pCbc->getCbcId()) << std::endl;
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


	void CbcInterface::WriteCbcReg(Cbc* pCbc, const std::string& pRegNode, uint32_t pValue, bool pVerifLoop)
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

        EncodeReg(cRegItem,pCbc->getCbcId(),cVecReq);

        fBoardFW->WriteCbcBlockReg(pCbc->getFeId(),cVecReq);

        if(pVerifLoop)
        {
            uint32_t cWriteValue, cReadValue;
            uint8_t cCbcId = pCbc->getCbcId();

            DecodeReg(cRegItem,cCbcId,cVecReq[0]);

            cWriteValue = cRegItem.fValue;
            cRegItem.fValue = 0;

            EncodeReg(cRegItem,pCbc->getCbcId(),cVecReq);

            fBoardFW->ReadCbcBlockReg(pCbc->getFeId(),cVecReq);

            DecodeReg(cRegItem,cCbcId,cVecReq[0]);

            cReadValue = cRegItem.fValue;

            if(cReadValue != cWriteValue)
            {
                std::cout << "ERROR !!!\nValues are not coinciding :\n" << "Written Value : " << cWriteValue << "\nReadback Value : " << cReadValue << std::endl;
				std::cout << "Register Adress : " << uint32_t(cRegItem.fAddress) << std::endl;
				std::cout << "Cbc Id : " << uint32_t(pCbc->getCbcId()) << std::endl;
				mypause();
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


	void CbcInterface::WriteCbcMultReg(Cbc* pCbc, std::vector< std::pair<std::string,uint32_t> > pVecReq, bool pVerifLoop)
	{

#ifdef __CBCDAQ_DEV__
		static long min(0), sec(0);
		struct timeval start0, end;
		long seconds(0), useconds(0);

		if(  DEV_FLAG ){
			gettimeofday(&start0, 0);
		}
#endif

		setBoard(pCbc->getBeId());

		std::vector<uint32_t> cVecReq;
		CbcRegItem cRegItem;

		for(uint32_t i=0; i<pVecReq.size(); i++)
		{
			cRegItem.fValue = pVecReq[i].second;
			cRegItem = (pCbc->getRegMap())[pVecReq[i].first];

			EncodeReg(cRegItem,pCbc->getCbcId(),cVecReq);

			pCbc->setReg(pVecReq[i].first,cRegItem.fValue);
		}

		fBoardFW->WriteCbcBlockReg(pCbc->getFeId(),cVecReq);

		if(pVerifLoop)
		{
			std::vector<uint32_t> cWriteValue, cReadValue, cVecReqBis;
			uint8_t cCbcId = pCbc->getCbcId();
			CbcRegItem cRegItem;

			for(int32_t i=0;i<cVecReq.size();i++)
			{
				DecodeReg(cRegItem,cCbcId,cVecReq[i]);

				cWriteValue.push_back(cRegItem.fValue);
				cRegItem.fValue = 0;

				EncodeReg(cRegItem,pCbc->getCbcId(),cVecReqBis);

				fBoardFW->ReadCbcBlockReg(pCbc->getFeId(),cVecReq);

				DecodeReg(cRegItem,cCbcId,cVecReqBis[0]);

				cReadValue.push_back(cRegItem.fValue);

				cVecReqBis.clear();

				if(cReadValue[i] != cWriteValue[i])
				{
					std::cout << "\nERROR !!!\nValues are not coinciding :\n" << "Written Value : " << cWriteValue[i] << "\nReadback Value : " << cReadValue[i] << std::endl;
					std::cout << "Register Adress : " << uint32_t(cRegItem.fAddress) << std::endl;
					std::cout << "Cbc Id : " << uint32_t(pCbc->getCbcId()) << std::endl;
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

		uint8_t cCbcId;
		CbcRegItem cRegItem = (pCbc->getRegMap())[pRegNode];
		std::vector<uint32_t> cVecReq;

		setBoard(pCbc->getBeId());

		EncodeReg(cRegItem,pCbc->getCbcId(),cVecReq);

		fBoardFW->ReadCbcBlockReg(pCbc->getFeId(),cVecReq);

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


	}


	void CbcInterface::ReadCbcMultReg(Cbc* pCbc,std::vector<std::string> pVecReg)
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
		CbcRegItem cRegItem;
		std::vector<uint32_t> cVecReq;

		setBoard(pCbc->getBeId());

		for(uint32_t i=0; i<pVecReg.size(); i++)
		{
			cRegItem = (pCbc->getRegMap())[pVecReg[i]];

			EncodeReg(cRegItem,pCbc->getCbcId(),cVecReq);

			fBoardFW->ReadCbcBlockReg(pCbc->getFeId(),cVecReq);

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


	void CbcInterface::ReadAllCbc(Module* pModule)
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

		setBoard(pModule->getBeId());

		for(uint8_t i=0;i<pModule->getNCbc();i++)
		{

            if(pModule->getCbc(i+cMissed) == NULL)
            {
                i--;
                cMissed++;
            }

            else
            {

                cCbc = pModule->getCbc(i+cMissed);

                CbcRegMap cCbcRegMap = cCbc->getRegMap();

                for(CbcRegMap::iterator cIt = cCbcRegMap.begin(); cIt != cCbcRegMap.end(); cIt++)
                {
                   EncodeReg(cIt->second,cCbc->getCbcId(),cVecReq);
                   cVecRegNode.push_back(cIt->first);
                }

                fBoardFW->ReadCbcBlockReg(cCbc->getFeId(),cVecReq);

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

				fBoardFW->WriteCbcBlockReg(cCbc->getFeId(),cVecReq);

				ReadCbcReg(pModule->getCbc(i+cMissed),pRegNode);
			}

			else
			{
				ReadCbcReg(pModule->getCbc(i+cMissed),pRegNode);
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

	void CbcInterface::CbcHardReset(Cbc* pCbc)
	{
		setBoard(pCbc->getBeId());

		fBoardFW->WriteReg(CBC_HARD_RESET,1);

		usleep(200000);

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


	void CbcInterface::EncodeReg(CbcRegItem& pRegItem, uint8_t pCbcId, std::vector<uint32_t>& pVecReq)
	{
		fBoardFW->EncodeReg(pRegItem, pCbcId, pVecReq);
	}


	void CbcInterface::DecodeReg(CbcRegItem& pRegItem, uint8_t pCbcId, uint32_t pWord)
	{
		fBoardFW->DecodeReg(pRegItem, pCbcId, pWord);
	}

}
