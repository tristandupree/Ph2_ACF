/*

  FileName :                     Utilities.cc
  Content :                      Some objects that might come in handy
  Programmer :                   Nicolas PIERRE
  Version :                      0.3
  Date of creation :             10/06/14
  Support :                      mail to : nicolas.pierre@cern.ch

*/
#include "Utilities.h"
#include "../HWDescription/Definition.h"
#include <iostream>

namespace Ph2_HwInterface
{

    //Get time took

    long getTimeTook( struct timeval &pStart, bool pMili )
    {
        struct timeval end;
        long seconds(0), useconds(0);

        gettimeofday( &end, 0 );
        seconds = end.tv_sec - pStart.tv_sec;
        useconds = end.tv_usec - pStart.tv_usec;

        if( pMili )
        {
                return (long) ( seconds * 1e3 + useconds/1000 );
        }

        else
        {
                return (long) (seconds * 1e6 + useconds );
        }
    }

    //--------------------------------------------------------------------------
    //Press enter function

    void myflush ( std::istream& in )
    {
        in.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
        in.clear();
    }

    void mypause()
    {
        std::cout<<"Press [Enter] to continue ...";
        std::cin.get();
    }


    //--------------------------------------------------------------------------
    // Swapping

    void swap_byte_order( const void *org, void *swapped, unsigned int nbyte )
    {
        for( unsigned int i=0; i<nbyte; i++ ){
            ((char *)swapped)[i] = ((char *)org)[nbyte-1-i];
        }
    }


    //--------------------------------------------------------------------------
    //Data Class

    Data::Data( Data &pD )
    {
		fBuf = 0;
		Initialise();
		fBufSize = pD.fBufSize;
	}


    void Data::Set( void *pData )
    {

        Reset();

		uhal::ValVector<uint32_t> *cUhalData = (uhal::ValVector<uint32_t>*)pData;

        for( unsigned int i=0; i<cUhalData->size(); i++ )
        {

			char cSwapped[4];
			uint32_t cVal = cUhalData->at(i);

			swapByteOrder( (const char *) &cVal, cSwapped, 4 );

			for( int j=0; j < 4; j++ ){
				fBuf[i*4+j] = cSwapped[j];
			}
		}
	}


    void Data::Initialise()
    {

		fNevents = EVENT_NUMBER;
		fBufSize = ( fNevents + 1 ) * PACKET_SIZE * 4;
		if( fBuf )
            free( fBuf );
		fBuf = (char *) malloc( fBufSize );

#ifdef __CBCDAQ_DEV__
		std::cout << "Data::Initialise done." << std::endl;
#endif

	}


    void Data::Reset()
    {
        for( uint32_t i=0; i<fBufSize; i++ )
            fBuf[i]=0;
    }


    void Data::CopyBuffer( Data &pData )
    {
		memcpy(fBuf,pData.fBuf,pData.fBufSize);
	}


    void Data::swapByteOrder( const char *org, char *swapped, unsigned int nbyte )
    {
		for( unsigned int i=0; i<nbyte; i++ )
        {
			swapped[i] = org[nbyte-1-i];
		}
	}

    const char * Data::GetBuffer( uint32_t &pBufSize ) const
    {
        pBufSize = fBufSize;
        return fBuf;
    }

    const Event *Data::GetNextEvent(){
		if( fCurrentEvent >= fNevents ) return 0;
		fEvent.SetEvent( &fBuf[ fCurrentEvent * EVENT_SIZE_32 * 4 ] );
		fCurrentEvent++;
		return &fEvent;
	}

}
