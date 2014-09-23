/*

	FileName :                     Event.cc
	Content :                      Event handling from DAQ
	Programmer :                   Nicolas PIERRE
	Version :                      1.0
	Date of creation :             10/07/14
	Support :                      mail to : nicolas.pierre@icloud.com

*/

#include "Event.h"

using namespace Ph2_HwDescription;


uint32_t swap_bytes( char* pOrg )
{
	uint32_t ui32 = pOrg[0] | (pOrg[1] << 8) | (pOrg[2] << 16) | (pOrg[3] << 24);
	return ((ui32 >> 24) & 0xFF) |
         ((ui32 >>  8) & 0x0000FF00) |
         ((ui32 <<  8) & 0x00FF0000) |
         ((ui32 << 24) & 0xFF000000);
}

namespace Ph2_HwInterface
{

	//Event implementation
	Event::Event(uint32_t pNbCbc)
	{
		SetSize(pNbCbc);
	}


	Event::Event( BeBoard& pBoard, uint32_t pNbCbc )
	{
		SetSize(pNbCbc);
		AddBoard(pBoard);
	}

	Event::Event(Event &pEvent):
		fBuf(0),
		fBunch(pEvent.fBunch),
		fOrbit(pEvent.fOrbit),
		fLumi(pEvent.fLumi),
		fEventCount(pEvent.fEventCount),
		fEventCountCBC(pEvent.fEventCountCBC),
		fTDC(pEvent.fTDC),
		fEventMap(pEvent.fEventMap)
	{

	}

	void Event::SetSize(uint32_t pNbCbc){
		if(pNbCbc == 2)
		{
			fEventSize = EVENT_SIZE_32_2CBC;
			fFeNChar = FE_NCHAR_2CBC;
			fOffsetTDC = OFFSET_TDC_2CBC;
		}
		else if(pNbCbc == 8)
		{
			fEventSize = EVENT_SIZE_32_8CBC;
			fFeNChar = FE_NCHAR_8CBC;
			fOffsetTDC = OFFSET_TDC_8CBC;
		}
		else
		{
			fEventSize = pNbCbc*2*9+6;
			fFeNChar = pNbCbc*CBC_NCHAR;
			fOffsetTDC = 5*32+9*2*pNbCbc*32;
		}
	}

	void Event::AddBoard( BeBoard& pBoard )
    {
		uint32_t cNFe = uint32_t(pBoard.getNFe());

		for(uint32_t i=0; i<cNFe; i++)
		{
			uint32_t cNCbc = uint32_t(pBoard.getModule(i)->getNCbc());
			FeEventMap cFeEventMap;

			for(uint32_t j=0; j<cNCbc; j++)
			{
				cFeEventMap[j] = 0;
			}

			fEventMap[i] = cFeEventMap;
		}

	}


	int Event::SetEvent( char *pEvent )
    {
		int vsize = sizeof(uint32_t);

        fBuf = pEvent;

		fBunch = swap_bytes(&pEvent[0*vsize]);

		fOrbit = swap_bytes(&pEvent[1*vsize]);

		fLumi = swap_bytes(&pEvent[2*vsize]);

		fEventCount = swap_bytes(&pEvent[3*vsize]);

		fEventCountCBC = swap_bytes(&pEvent[4*vsize]);

		fTDC = swap_bytes(&pEvent[fOffsetTDC]);


		for(EventMap::iterator cIt = fEventMap.begin() ; cIt != fEventMap.end(); cIt++ )
		{
			uint8_t FeId = uint8_t(cIt->first);

			for(FeEventMap::iterator cJt = cIt->second.begin(); cJt != cIt->second.end(); cJt++ )
			{
				uint8_t CbcId = uint8_t(cJt->first);
				cJt->second = &pEvent[OFFSET_FE_EVENT + FeId * fFeNChar + CbcId * CBC_NCHAR];
			}
		}

		return 0;
	}


	char* Event::GetCbcEvent( uint8_t& pFeId, uint8_t& pCbcId ) const
    {
		EventMap::const_iterator cIt = fEventMap.find(pFeId);

    	if( cIt == fEventMap.end() )
        {
			std::cout << "Event: FE " << pFeId << " is not found." << std::endl;
			return NULL;
		}

		FeEventMap::const_iterator cJt = cIt->second.find(pCbcId);

		if( cJt == cIt->second.end() )
		{
			std::cout << "Event: CBC " << pCbcId << " is not found." << std::endl;
			return NULL;
		}

        return cJt->second;
	}


	std::string Event::HexString() const
    {
    	std::stringbuf tmp;
		std::ostream os(&tmp);

		os <<std::hex;

    	for( uint32_t i=0; i<fEventSize; i++)
        {
			os <<std::uppercase<<std::setw(2)<<std::setfill('0')<< (fBuf[i]&0xFF);
		}

        os << std::endl;

        return tmp.str();
	}


	bool Event::Bit(uint8_t pFeId, uint8_t pCbcId, uint32_t pPosition ) const
    {
		uint32_t cByteP=pPosition/8;
		uint32_t cBitP =pPosition%8;

		return GetCbcEvent(pFeId, pCbcId)[cByteP] & (1<<(7-cBitP));
	}


	bool Event::Error(uint8_t pFeId, uint8_t pCbcId, uint32_t i ) const
    {
		return Bit(pFeId, pCbcId, i+OFFSET_ERROR );
	}

	uint32_t Event::Error(uint8_t pFeId, uint8_t pCbcId) const
    {

		uint32_t val(0);
		uint32_t cWidth = WIDTH_ERROR;

        for( uint32_t i=0; i<cWidth; i++ )
        {
			val = val<<1; val |= Error(pFeId, pCbcId,i);
		}

        return val;
	}


	uint32_t Event::PipelineAddress(uint8_t pFeId, uint8_t pCbcId) const
    {
		uint32_t cOffset = OFFSET_PIPELINE_ADDRESS;
		uint32_t cWidth = WIDTH_PIPELINE_ADDRESS;
		uint32_t val(0);

    	for( uint32_t i=0; i<cWidth; i++ )
        {
			val = val<<1; val |= Bit(pFeId, pCbcId, cOffset+i );
		}

        return val;
	}

	bool Event::DataBit(uint8_t pFeId, uint8_t pCbcId, uint32_t i) const
    {
		if( i > NSENSOR )
            return 0;

        return Bit(pFeId, pCbcId, i+OFFSET_CBCDATA );
	}


	std::string Event::BitString(uint8_t pFeId, uint8_t pCbcId, uint32_t pOffset, uint32_t pWidth ) const
    {
		std::stringbuf tmp;
		std::ostream os(&tmp);

        for( uint32_t i=0; i<pWidth; i++ )
            os << Bit(pFeId, pCbcId, i+pOffset );

        return tmp.str();
	}


	std::vector<bool> Event::BitVector(uint8_t pFeId, uint8_t pCbcId, uint32_t pOffset, uint32_t pWidth ) const
	{
		std::vector<bool> tmp;

		for( uint32_t i=0; i<pWidth; i++ )
		{
			tmp.push_back(Bit(pFeId, pCbcId, i+pOffset ));
		}

		return tmp;
	}


	std::string Event::DataBitString(uint8_t pFeId, uint8_t pCbcId) const
    {
		return BitString(pFeId, pCbcId, OFFSET_CBCDATA, WIDTH_CBCDATA );
	}


	std::vector<bool> Event::DataBitVector(uint8_t pFeId, uint8_t pCbcId) const
	{
		return BitVector(pFeId, pCbcId, OFFSET_CBCDATA, WIDTH_CBCDATA );
	}


	std::string Event::DataHexString(uint8_t pFeId, uint8_t pCbcId) const
    {
		std::stringbuf tmp;
		std::ostream os(&tmp);

		os << std::hex;

		uint32_t cFirstByteP = OFFSET_CBCDATA/8;
		uint32_t cFirstBitP = OFFSET_CBCDATA%8;
		uint32_t cLastByteP = (cFirstByteP+WIDTH_CBCDATA-1)/8;
		uint32_t cLastBitP = (cFirstByteP+WIDTH_CBCDATA-1)%8;

		uint32_t cMask(0);
		uint32_t cMaskLastBit(0);
		uint32_t cMaskWidth(0);

    	//First byte
		cMaskLastBit = cFirstByteP < cLastByteP ? 7 : cLastBitP;
		cMaskWidth = cMaskLastBit - cFirstBitP + 1;
		cMask = ( 1 << ( 7 - cMaskLastBit ) );

        for( uint32_t i=0;i < cMaskWidth; i++ )
            {
                cMask = cMask << 1;
                cMask |= 1;
            }

        os <<std::uppercase<<std::setw(2)<<std::setfill('0')<< (GetCbcEvent(pFeId, pCbcId)[cFirstByteP]&cMask);

    	if( cFirstByteP == cLastByteP )
            return tmp.str();

    	//Second to the second last byte
		if( cFirstByteP != cLastByteP - 1 )
        {
			for( uint32_t j=cFirstByteP+1; j<cLastByteP; j++ )os <<std::uppercase<<std::setw(2)<<std::setfill('0')<< (GetCbcEvent(pFeId, pCbcId)[j] & 0xFF);
		}

		//Last byte
		cMaskLastBit = cLastBitP;
		cMaskWidth = cMaskLastBit+1;
		cMask = ( 1 << ( 7 - cMaskLastBit ) );

        for( uint32_t i=0;i < cMaskWidth; i++ )
            {
                cMask = cMask << 1;
                cMask |= 1;
            }

		os <<std::uppercase<<std::setw(2)<<std::setfill('0')<< (GetCbcEvent(pFeId, pCbcId)[cFirstByteP]&cMask);

        return tmp.str();
	}


	std::string Event::GlibFlagString(uint8_t pFeId, uint8_t pCbcId) const
    {
		return BitString(pFeId, pCbcId, OFFSET_GLIBFLAG, WIDTH_GLIBFLAG );
	}


	std::string Event::StubBitString(uint8_t pFeId, uint8_t pCbcId) const
    {
		return BitString(pFeId, pCbcId, OFFSET_CBCSTABDATA, WIDTH_CBCSTABDATA );
	}

}
