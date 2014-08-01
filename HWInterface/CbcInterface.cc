/*!
*
* \file CBCInterface.cc
* \brief CBCInterface.cc User interface for the cbc's
* \author Lorenzo BIDEGAIN, Nicolas Pierre
* \date 30/07/14
*
* Support : mail to : lorenzo.bidegain@cern.ch, nicolas.pierre@cern.ch
*
*/

#include "CbcInterface.h"

namespace Ph2_HwInterface
{

	CbcInterface::CbcInterface(BeBoardFWMap& pBoardMap):fBoardMap(pBoardMap)
	{
		fBoardFW=NULL;
		prevBoardId=255;
	}

	CbcInterface::~CbcInterface()
	{
		
	}

	void CbcInterface::setBoard(uint8_t pBoardId)
	{
		if(prevBoardId==pBoardId)
			return;
		else
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
		

	void CbcInterface::ConfigureCbc(Cbc* pCbc)
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
        	pCbc->loadfRegMap(DEFAULT_FILE);
		CbcRegMap cCbcRegMap = pCbc->getRegMap();

		for(CbcRegMap::iterator cIt = cCbcRegMap.begin(); cIt != cCbcRegMap.end(); cIt++)
		{
			EncodeReg(cIt->second,pCbc->fCbcId,cVecReq);
		}

		fBoardFW->WriteCbcBlockReg(pCbc,cVecReq);

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


	void CbcInterface::WriteCbcReg(Cbc* pCbc, const std::string& pRegNode, uint8_t pValue)
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

		CbcRegItem cRegItem = (pCbc->getRegMap())[pRegNode];
		std::vector<uint32_t> cVecReq;

		cRegItem.fValue = pValue;

		fBoardFW->ChooseBoard(pCbc->getBeId());

		EncodeReg(cRegItem,pCbc->fCbcId,cVecReq);

		fBoardFW->WriteCbcBlockReg(pCbc,cVecReq);

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

	
	void CbcInterface::ReadCbcReg(Cbc* pCbc,const std::string& pRegNode)
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

		uint8_t cCbcId;
		CbcRegItem cRegItem = (pCbc->getRegMap())[pRegNode];
		std::vector<uint32_t> cVecReq;

        	std::cout << uint32_t(cRegItem.fValue) << std::endl;

		fBoardFW->ChooseBoard(pCbc->getBeId());

		EncodeReg(cRegItem,pCbc->fCbcId,cVecReq);

		fBoardFW->ReadCbcBlockReg(pCbc,cVecReq);

		DecodeReg(cRegItem,cCbcId,cVecReq[0]);
        	std::cout << uint32_t(cCbcId) << std::endl;
        	std::cout << uint32_t(cRegItem.fValue) << std::endl;


		pCbc->setReg(pRegNode,cRegItem.fValue);

		#ifdef __CBCDAQ_DEV__
		if(  DEV_FLAG )
		{
			gettimeofday( &end, 0 );
			seconds = end.tv_sec - start0.tv_sec;
			useconds = end.tv_usec - start0.tv_usec;
			min += ( seconds + useconds / 1000000 ) /60;
			sec += ( seconds + useconds / 1000000 ) %60;
			std::cout << "Time took for Cbc register refresh so far = " << min << " min " << sec << " sec." << std::endl;
		}
		#endif

	}


	void CbcInterface::ReadCbc(Module* pModule,const std::string& pRegNode)
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

		int cMissed = 0;

		for(uint8_t i=0;i<pModule->getNCbc();i++)
		{

            		if(pModule->getCbc(i+cMissed) == NULL)
            		{
                		i--;
                		cMissed++;
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
			std::cout << "Time took for reading all Cbcs so far = " << min << " min " << sec << " sec." << std::endl;
		}
		#endif

	}


	void CbcInterface::WriteBroadcast(Module* pModule,const std::string& pRegNode, uint8_t pValue)
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

				fBoardFW->WriteCbcBlockReg(cCbc,cVecReq);

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
		
		fBoardFW->ChooseBoard(pCbc->getBeId());

		fBoardFW->WriteReg(CBC_HARD_RESET,1);

		usleep(200000);

		fBoardFW->WriteReg(CBC_HARD_RESET,0);

		usleep(200000);
	}

	void CbcInterface::CbcFastReset(Cbc* pCbc)
	{
		setBoard(pCbc->getBeId());
		
		fBoardFW->ChooseBoard(pCbc->getBeId());

		fBoardFW->WriteReg(CBC_FAST_RESET,1);

		usleep(200000);

		fBoardFW->WriteReg(CBC_FAST_RESET,0);
	}

}
