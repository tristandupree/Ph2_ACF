/*

    FileName :                     Data.h
    Content :                      Data handling from DAQ
    Programmer :                   Nicolas PIERRE
    Version :                      0.4
    Date of creation :             10/07/14
    Support :                      mail to : nicolas.pierre@icloud.com

*/

#ifndef __DATA_H__
#define __DATA_H__

#include <uhal/uhal.hpp>
#include <ios>
#include <istream>
#include "Event.h"
#include "../HWDescription/BeBoard.h"

using namespace Ph2_HwDescription;

namespace Ph2_HwInterface
{

    /*!
    * \class Data
    * \brief Dummy data format in order to test
    */
    class Data
    {
        private:
            char *fBuf;
            uint32_t fBufSize;
            uint32_t fNevents;
            Event fEvent;
            uint32_t fCurrentEvent;

        private:
            void swapByteOrder( const char *org, char *swapped, unsigned int nbyte );

        public:
            Data():fBuf(0),fCurrentEvent(0) {}
            Data(BeBoard& pBoard);
            Data(Data &pData);
            ~Data() {if(fBuf) free(fBuf);}

            void Initialise( uint32_t pNevents );
            void Initialise( uint32_t pNevents, BeBoard& pBoard );
            void Set( void *pData );
            void Reset();
            void CopyBuffer( Data &pData );
            const char * GetBuffer( uint32_t &pBufSize ) const;
            const Event * GetNextEvent();

    };

}

#endif
