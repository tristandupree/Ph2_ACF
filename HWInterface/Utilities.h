/*

    \file                          Utilities.h
    \brief                         Some objects that might come in handy
    \author                        Nicolas PIERRE
    \version                       1.0
    \date                          10/06/14
    Support :                      mail to : nicolas.pierre@icloud.com

 */

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <sys/time.h>
#include <ios>
#include <istream>
#include <limits>

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

}

#endif
