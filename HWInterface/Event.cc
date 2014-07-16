#include "Event.h"


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
			fEventMap[i] = 0;
			uint8_t cNCbc = pBoard.getModule(i)->getNCbc();

			for(int j=0; j<cNCbc; j++)
			{
				fEventMap[i][j] = 0;
			}
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
			uint32_t FeId = uint32_t(cIt->first);

			for(FeEventMap::iterator cJt = cIt->second.begin(); cJt != cIt->second.end(); cJt++ )
			{
				uint32_t CbcId = uint32_t(cJt->first);
				cJt->second = &pEvent[OFFSET_FE_EVENT + FeId * FE_NCHAR + CbcId * CBC_NCHAR] );
			}
		}

		return 0;
	}


	char* Event::GetCbcEvent( uint32_t pFeId, uint32_t pCbcId )
    {
		EventMap::iterator cIt = fEventMap.find(pFeId);

    	if( cIt == fEventMap.end() )
        {
			std::cout << "Event: FE " << pFeId << " is not found." << std::endl;
			return 0;
		}

		FeEventMap::iterator cJt = cIt->second.find(pCbcId);

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

}
