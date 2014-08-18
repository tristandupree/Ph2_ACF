/*!

	Filename : 				Shelve.cc
	Content : 				Shelve Description class, handles a vector of Board
	Programmer : 			Lorenzo BIDEGAIN
	Version :				1.0
	Date of Creation : 		11/08/14
	Support : 				mail to : lorenzo.bidegain@gmail.com

*/

#include "Shelve.h"

namespace Ph2_HwDescription{

	Shelve::Shelve( uint8_t pShelveId)
	{
		fShelveId=pShelveId;
	}

	Shelve::Shelve()
	{
		fShelveId=0;
	}


	void Shelve::addBoard( BeBoard& pBoard )
	{
		fBoardVector.push_back( pBoard );
	}


	bool Shelve::removeBoard( uint8_t pBeId )
	{
		std::vector < BeBoard > :: iterator i;
		bool j=false;
		for (i=fBoardVector.begin();i!=fBoardVector.end();++i)
		{
			if (i->fBeId==pBeId)
			{
				fBoardVector.erase(i);
				j=true;
				i--;   //erase reduces the container size by the number of elements removed, which are destroyed. To avoid that the iterator point an unallocated part of the memory, we need to decrease the iterator
			}
		}
		if (j==true)
		return true;
		else
		{
			std::cout<<"Error:This Shelve doesn't have the Board"<<std::endl;
			return false;
		}
	}


	BeBoard*   Shelve::getBoard( uint8_t pBeId )
	{
		std::vector < BeBoard > :: iterator i;
		for (i=fBoardVector.begin();i!=fBoardVector.end();++i)
		{
			if (i->fBeId==pBeId)
				{
					return &*i ;
				}
		}
		return NULL;

	}

}
