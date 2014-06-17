/*

  FileName : 					Exception.cc
  Content : 					 Handles with exceptions
  Programmer : 				  Nicolas PIERRE
  Version : 					 1.0
  Date of creation : 	        10/06/14
  Support : 					 mail to : nicolas.pierre@cern.ch

*/

#include "Exception.h"
#include "TROOT.h"

namespace Ph2_HwInterface
{
        const char *Exception::what() const throw()
        {
            return Form( "CBCInterface %s", fStrError.c_str() );
        }
}
