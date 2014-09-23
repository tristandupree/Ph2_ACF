/*

    \file                          Data.h
    \brief                         Data handling from DAQ
    \author                        Nicolas PIERRE
    \version                       1.0
    \date                          10/07/14
    Support :                      mail to : nicolas.pierre@icloud.com

*/

#ifndef __DATA_H__
#define __DATA_H__

#include <uhal/uhal.hpp>
#include <ios>
#include <istream>
#include "Event.h"
#include "../HWDescription/BeBoard.h"


#define SWAP_4(x) ( ((x) << 24) | \
         (((x) << 8) & 0x00ff0000) | \
         (((x) >> 8) & 0x0000ff00) | \
         ((x) >> 24) )

#define SWAP_2(x) ( (((x) & 0xff) << 8) | ((unsigned short)(x) >> 8) )


using namespace Ph2_HwDescription;
namespace Ph2_HwInterface
{

    /*!
    * \class Data
    * \brief Data container to manipulate data flux from the Cbc
    */
    class Data
    {
        private:
            char *fBuf; /*! Data buffer <*/
            uint32_t fBufSize; /*! Size of Data buffer <*/
            uint32_t fNevents; /*! Number of Events<*/
            Event fEvent; /*! Events container <*/
            uint32_t fCurrentEvent; /*! Current Event in use <*/

        private:
            /*!
            * \brief Byte swapping (not used)
            * \param *org : input byte string
            * \param *swapped : output byte string
            * \param nbytes : lenght of byte string
            */
            void swapByteOrder( const char *org, char *swapped, unsigned int nbyte );

        public:
            /*!
            * \brief Constructor of the Data class
            * \param pNbCbc
            */
            Data(uint32_t pNbCbc):fBuf(0),fCurrentEvent(0),fEvent(pNbCbc) {}
            /*!
            * \brief Constructor of the Data class
            * \param pBoard : Board to work with
            * \param pNbCbc
            */
            Data(BeBoard& pBoard, uint32_t pNbCbc);
            /*!
            * \brief Copy Constructor of the Data class
            */
            Data(Data &pData);
            /*!
            * \brief Destructor of the Data class
            */
            ~Data() {if(fBuf) free(fBuf);}

            /*!
            * \brief Initialise the data structure
            * \param pNevents : number of Events
            */
            void Initialise( uint32_t pNevents );
            /*!
            * \brief Initialise the data structure
            * \param pNevents : number of Events
            * \param pBoard : Board to work with
            */
            void Initialise( uint32_t pNevents, BeBoard& pBoard );
            /*!
            * \brief Set the data in the data map
            * \param *pData : Data from the Cbc
            */
            void Set( void *pData );
            /*!
            * \brief Reset the data structure
            */
            void Reset();
            /*!
            * \brief Copy the data buffer
            * \param pData : Data to copy in
            */
            void CopyBuffer( Data &pData );
            /*!
            * \brief Copy the data buffer
            * \param pBufSize : size of the buffer
            * \return Data buffer
            */
            const char * GetBuffer( uint32_t &pBufSize ) const;
            /*!
            * \brief Get the next Event
            * \return Next Event
            */
            const Event * GetNextEvent();

    };

}

#endif
