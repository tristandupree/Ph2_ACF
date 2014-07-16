#include "Event.h"

using namespace Ph2_HwDescription;

void swap_byte_order( const void *org, void *swapped, unsigned int nbyte )
{
	for( unsigned int i=0; i<nbyte; i++ )
    {
		((char *)swapped)[i] = ((char *)org)[nbyte-1-i];
	}
}

namespace Ph2_HwInterface
{

	//Event implementation

	Event::Event(Event &pEvent):
		fBuf(0),
		fBunch(pEvent.fBunch),
		fOrbit(pEvent.fOrbit),
		fLumi(pEvent.fLumi),
		fEventCount(pEvent.fEventCount),
		fEventCountCBC(pEvent.fEventCountCBC),
		fTDC(pEvent.fTDC)
	{

	}


	void Event::AddBoard( BeBoard& pBoard )
    {
		uint8_t cNFe = pBoard.getNFe();
		for(int i=0; i<cNFe; i++)
		{
			uint8_t cNCbc = pBoard.getModule(i)->getNCbc();
			FeEventMap cFeEventMap;

			for(int j=0; j<cNCbc; j++)
			{
				cFeEventMap[j] = 0;
			}

			fEventMap[i] = cFeEventMap;
		}
	}


	int Event::SetEvent( char *pEvent )
    {
		int vsize = sizeof(uint32_t);
        uint32_t swapped(0);

        fBuf = pEvent;

		swap_byte_order( &pEvent[0*vsize], &swapped, vsize );
		fBunch = swapped & 0xFFFFFF ;

        swap_byte_order( &pEvent[1*vsize], &swapped, vsize );
		fOrbit = swapped & 0xFFFFFF ;

        swap_byte_order( &pEvent[2*vsize], &swapped, vsize );
		fLumi = swapped & 0xFFFFFF ;

        swap_byte_order( &pEvent[3*vsize], &swapped, vsize );
		fEventCount = swapped & 0xFFFFFF ;

        swap_byte_order( &pEvent[4*vsize], &swapped, vsize );
		fEventCountCBC = swapped & 0xFFFFFF ;

        swap_byte_order( &pEvent[OFFSET_TDC], &swapped, vsize );
		fTDC = swapped & 0xFFFFFF ;


		for(EventMap::iterator cIt = fEventMap.begin() ; cIt != fEventMap.end(); cIt++ )
		{
			uint8_t FeId = uint8_t(cIt->first);

			for(FeEventMap::iterator cJt = cIt->second.begin(); cJt != cIt->second.end(); cJt++ )
			{
				uint8_t CbcId = uint8_t(cJt->first);
				cJt->second = &pEvent[OFFSET_FE_EVENT + FeId * FE_NCHAR + CbcId * CBC_NCHAR];
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
			return 0;
		}

		FeEventMap::const_iterator cJt = cIt->second.find(pCbcId);

		if( cJt == cIt->second.end() )
		{
			std::cout << "Event: CBC " << pCbcId << " is not found." << std::endl;
			return 0;
		}

        return cJt->second;
	}


	std::string Event::HexString() const
    {
    	std::stringbuf tmp;
		std::ostream os(&tmp);

		os <<std::hex;

    	for( uint32_t i=0; i<EVENT_SIZE_32; i++)
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


	std::string Event::DataBitString(uint8_t pFeId, uint8_t pCbcId) const
    {
		return BitString(pFeId, pCbcId, OFFSET_CBCDATA, WIDTH_CBCDATA );
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
