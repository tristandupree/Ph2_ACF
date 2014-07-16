/*

  FileName :                     Utilities.h
  Content :                      Some objects that might come in handy
  Programmer :                   Nicolas PIERRE
  Version :                      0.3
  Date of creation :             10/06/14
  Support :                      mail to : nicolas.pierre@cern.ch

*/
#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <uhal/uhal.hpp>
#include <sys/time.h>
#include <ios>
#include <istream>
#include <limits>
#include "Event.h"

/*!
* \namespace Ph2_HwInterface
* \brief Namespace regrouping all the interfaces to the hardware
*/
namespace Ph2_HwInterface
{
    /*!
    * \brief Get time took since the start
    * \param pStart : Variable taking the start
    * \param pMili : Result in milliseconds/microseconds -> 1/0
    * \return The time took
    */
    long getTimeTook( struct timeval &pStart, bool pMili );
    /*!
    * \brief Flush the content of the input stream
    * \param in : input stream
    */
    void myflush ( std::istream& in );
    /*!
    * \brief Wait for Enter key press
    */
    void mypause ();

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
            Data():fBuf(0) {}
            Data(Data &pData);
            ~Data() {if(fBuf) free(fBuf);}

            void Initialise();
            void Set( void *pData );
            void Reset();
            void CopyBuffer( Data &pData );
            const char * GetBuffer( uint32_t &pBufSize ) const;
            const Event * GetNextEvent();

    };

}

#endif
