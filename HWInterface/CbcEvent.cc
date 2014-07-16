#include "CbcEvent.h"
#include <iomanip>

namespace Ph2_HwInterface
{
	/*
	   CbcEvent implementation
	   fBuf defined in CBC2 specification + GLIB internals + STUB bits
	   Error(00) Address(8) Data(254) GLIB(12) STUB(12)
	 */
	const uint32_t CbcEvent::NSENSOR = 254;

	const uint32_t CbcEvent::OFFSET_ERROR = 0;
	const uint32_t CbcEvent::WIDTH_ERROR = 2;
	const uint32_t CbcEvent::OFFSET_PIPELINE_ADDRESS = OFFSET_ERROR + WIDTH_ERROR;
	const uint32_t CbcEvent::WIDTH_PIPELINE_ADDRESS = 8;
	const uint32_t CbcEvent::OFFSET_CBCDATA = OFFSET_PIPELINE_ADDRESS + WIDTH_PIPELINE_ADDRESS;
	const uint32_t CbcEvent::WIDTH_CBCDATA = NSENSOR;
	const uint32_t CbcEvent::OFFSET_GLIBFLAG = OFFSET_CBCDATA + WIDTH_CBCDATA;
	const uint32_t CbcEvent::WIDTH_GLIBFLAG = 12;
	const uint32_t CbcEvent::OFFSET_CBCSTABDATA = OFFSET_GLIBFLAG + WIDTH_GLIBFLAG;
	const uint32_t CbcEvent::WIDTH_CBCSTABDATA = 12;


    CbcEvent::CbcEvent(uint32_t pFeId, uint32_t pId):
		fFeId(pFeId),
		fCbcId(pId),
		fBuf(0)
    {

    }


	CbcEvent::CbcEvent(const CbcEvent& pEvent):
		fFeId(pEvent.fFeId),
		fCbcId(pEvent.fCbcId),
		fBuf(0)
    {

	}


	void CbcEvent::Set( char *pEvent )
    {
        fBuf = pEvent;
    }


	bool CbcEvent::Error( uint32_t i ) const
    {
		return Bit( i+OFFSET_ERROR );
	}


	uint32_t CbcEvent::Error() const
    {

		uint32_t val(0);
		uint32_t cWidth = WIDTH_ERROR;

        for( uint32_t i=0; i<cWidth; i++ )
        {
			val = val<<1; val |= Error(i);
		}

        return val;
	}


	uint32_t CbcEvent::PipelineAddress() const
    {
		uint32_t cOffset = OFFSET_PIPELINE_ADDRESS;
		uint32_t cWidth = WIDTH_PIPELINE_ADDRESS;
		uint32_t val(0);

    	for( uint32_t i=0; i<cWidth; i++ )
        {
			val = val<<1; val |= Bit( cOffset+i );
		}

        return val;
	}


	bool CbcEvent::DataBit(uint32_t i) const
    {
		if( i > NSENSOR )
            return 0;

        return Bit( i+OFFSET_CBCDATA );
	}


    //function to get bit string of CBC data
	std::string CbcEvent::DataBitString() const
    {
		return BitString( OFFSET_CBCDATA, WIDTH_CBCDATA );
	}


    //function to get bit string in hexadecimal format for CBC data
	std::string CbcEvent::DataHexString() const
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

        os <<std::uppercase<<std::setw(2)<<std::setfill('0')<< (fBuf[cFirstByteP]&cMask);

    	if( cFirstByteP == cLastByteP )
            return tmp.str();

    	//Second to the second last byte
		if( cFirstByteP != cLastByteP - 1 )
        {
			for( uint32_t j=cFirstByteP+1; j<cLastByteP; j++ )os <<std::uppercase<<std::setw(2)<<std::setfill('0')<< (fBuf[j] & 0xFF);
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

		os <<std::uppercase<<std::setw(2)<<std::setfill('0')<< (fBuf[cFirstByteP]&cMask);

        return tmp.str();
	}


    //function to get GLIB flag string
	std::string CbcEvent::GlibFlagString() const
    {
		return BitString( OFFSET_GLIBFLAG, WIDTH_GLIBFLAG );
	}


	//function to get bit string for CBC STAB data
	std::string CbcEvent::StubBitString() const
    {
		return BitString( OFFSET_CBCSTABDATA, WIDTH_CBCSTABDATA );
	}


    //function to get the bit at the global data string position
	bool CbcEvent::Bit( uint32_t pPosition ) const
    {
		uint32_t cByteP=pPosition/8;
		uint32_t cBitP =pPosition%8;
		return fBuf[cByteP] & (1<<(7-cBitP));
	}


    //function to get bit string from the data offset and width
	std::string CbcEvent::BitString( uint32_t pOffset, uint32_t pWidth ) const
    {
		std::stringbuf tmp;
		std::ostream os(&tmp);

        for( uint32_t i=0; i<pWidth; i++ )
            os << Bit( i+pOffset );

        return tmp.str();
	}
}
