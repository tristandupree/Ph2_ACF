/*

  FileName : 					Utilities.cc
  Content : 					 Some objects that might come in handy
  Programmer : 				  Nicolas PIERRE
  Version : 					 1.0
  Date of creation : 	        10/06/14
  Support : 					 mail to : nicolas.pierre@cern.ch

*/
#include "Utilities.h"
#include <iostream>


namespace Ph2_HwInterface
{
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

}
