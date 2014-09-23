/*

    FileName :                     Data.cc
    Content :                      Data handling from DAQ
    Programmer :                   Nicolas PIERRE
    Version :                      1.0
    Date of creation :             10/07/14
    Support :                      mail to : nicolas.pierre@icloud.com

*/

#include "Data.h"
#include "../HWDescription/Definition.h"
#include <iostream>

namespace Ph2_HwInterface
{

    //--------------------------------------------------------------------------
    // Swapping

    // void swap_byte_order( const void *org, void *swapped, unsigned int nbyte )
    // {
    //     for( unsigned int i=0; i<nbyte; i++ ){
    //         ((char *)swapped)[i] = ((char *)org)[nbyte-1-i];
    //     }
    // }

    // Have to see if this actually works
    // void swap_byte_order( const void *org, void *swapped, unsigned int nbyte )
    // {
    //    // swapped = ((org >> 24) & 0xFF) | ((org >> 8) & 0x0000FF00) | ((org << 8) & 0x00FF0000) | ((org << 24) & 0xFF000000);
    //     // static const unsigned char t[16] =
    //     // {
    //     //   0x0, 0x8, 0x4, 0xC, 0x2, 0xA, 0x6, 0xE,
    //     //   0x1, 0x9, 0x5, 0xD, 0x3, 0xB, 0x7, 0xF
    //     // };
    //     // t[*(char*)org >> nbyte] | (t[*(char*)org & 0xF] << nbyte);
    //     // (char*) swapped = &t;
    //     (char*) swapped[0] = (char*)org[3];
    //     (char*) swapped[1] = (char*)org[2];
    //     (char*) swapped[2] = (char*)org[1];
    //     (char*) swapped[3] = (char*)org[0];

    // }

    //--------------------------------------------------------------------------
    //Data Class

    Data::Data(BeBoard& pBoard, uint32_t pNbCbc):
        fBuf(0),
        fCurrentEvent(0),
        fEvent(pNbCbc)
    {
        fEvent.AddBoard(pBoard);
    }

    Data::Data( Data &pD ):
        fEvent(0)
    {
        fBuf = 0;
        Initialise( pD.fNevents);
        fEvent = pD.fEvent;
        fBufSize = pD.fBufSize;
        fNevents = pD.fNevents;
        fCurrentEvent = pD.fCurrentEvent;
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


    void Data::Initialise( uint32_t pNevents )
    {

        fNevents = pNevents;
        fBufSize = ( fNevents + 1 ) * fEvent.fEventSize * 4;
        if( fBuf )
            free( fBuf );
        fBuf = (char *) malloc( fBufSize );

        fEvent.Clear();

#ifdef __CBCDAQ_DEV__
        std::cout << "Data::Initialise done." << std::endl;
#endif

    }


    void Data::Initialise( uint32_t pNevents, BeBoard& pBoard )
    {

        fNevents = pNevents;
        fBufSize = ( fNevents + 1 ) * fEvent.fEventSize * 4;
        if( fBuf )
            free( fBuf );
        fBuf = (char *) malloc( fBufSize );

        fEvent.Clear();

        fEvent.AddBoard(pBoard);

#ifdef __CBCDAQ_DEV__
        std::cout << "Data::Initialise done." << std::endl;
#endif

    }


    void Data::Reset()
    {
        fCurrentEvent = 0;

        for( uint32_t i=0; i<fBufSize; i++ )
            fBuf[i]=0;
    }


    void Data::CopyBuffer( Data &pData )
    {
        memcpy(fBuf,pData.fBuf,pData.fBufSize);
    }


    // void Data::swapByteOrder( const char *org, char *swapped, unsigned int nbyte )
    // {
    //     for( unsigned int i=0; i<nbyte; i++ )
    //     {
    //         swapped[i] = org[nbyte-1-i];
    //     }
    // }

    void Data::swapByteOrder( const char *org, char *swapped, unsigned int nbyte )
    {
        swapped[0] = org[3];
        swapped[1] = org[2];
        swapped[2] = org[1];
        swapped[3] = org[0];
    }


    const char * Data::GetBuffer( uint32_t &pBufSize ) const
    {
        pBufSize = fBufSize;
        return fBuf;
    }


    const Event *Data::GetNextEvent()
    {
        if( fCurrentEvent >= fNevents ) return 0;
        fEvent.SetEvent( &fBuf[ fCurrentEvent * fEvent.fEventSize * 4 ] );
        fCurrentEvent++;
        return &fEvent;
    }

}
