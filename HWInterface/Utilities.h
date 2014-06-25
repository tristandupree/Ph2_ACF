/*

  FileName : 					Utilities.h
  Content : 					 Some objects that might come in handy
  Programmer : 				  Nicolas PIERRE
  Version : 					 1.0
  Date of creation : 	        10/06/14
  Support : 					 mail to : nicolas.pierre@cern.ch

*/
#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <sys/time.h>

namespace Ph2_HwInterface
{
    long getTimeTook( struct timeval &pStart, bool pMili );
}

#endif
