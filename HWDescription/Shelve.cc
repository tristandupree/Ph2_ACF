/*!

        Filename :                              Shelve.cc
        Content :                               Shelve Description class, handles a vector of Board
        Programmer :                    Lorenzo BIDEGAIN
        Version :               1.0
        Date of Creation :              11/08/14
        Support :                               mail to : lorenzo.bidegain@gmail.com

 */

#include "Shelve.h"

namespace Ph2_HwDescription
{

	Shelve::Shelve( uint8_t pShelveId )
	{
		fShelveId = pShelveId;
	}

	Shelve::Shelve()
	{
		fShelveId = 0;
	}

	bool Shelve::removeBoard( uint8_t pBeId )
	{
		std::vector < BeBoard* > :: iterator i;
		bool found = false;
		for ( i = fBoardVector.begin(); i != fBoardVector.end(); ++i )
		{
		        if ( (*i)->getBeId() == pBeId )
			{
				found = true;
                                break;  
			}
		}
		if ( found ) {
		        fBoardVector.erase( i );
			return true;
                }
		else
		{
			std::cout << "Error:The Shelve :" << +fShelveId 
                                  << " doesn't have the Board " << +pBeId << std::endl;
			return false;
		}
	}


	BeBoard*   Shelve::getBoard( uint8_t pBeId ) const
	{
		for ( std::vector < BeBoard* > :: const_iterator i = fBoardVector.begin(); i != fBoardVector.end(); ++i )
		{
	 	        if ( (*i)->getBeId() == pBeId )
				return *i;
		}
		return NULL;

	}
}
