/*!
*
* \file Module.cc
* \brief Module Description class
* \author Lorenzo BIDEGAIN
* \date 25/06/14
*
* Support : mail to : lorenzo.bidegain@cern.ch
*
*/


#include "Module.h"

namespace Ph2_HwDescription{

	// Default C'tor
	Module::Module():FrontEndDescription(){fModuleId=0;}

	Module::Module( FrontEndDescription& pFeDesc, uint8_t pModuleId):FrontEndDescription(pFeDesc){fModuleId=pModuleId;
	}

	Module::Module( uint8_t pShelveId, uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId, uint8_t pModuleId ):FrontEndDescription( pShelveId, pBeId, pFMCId,pFeId ){fModuleId=pModuleId;
	}

	void Module::addCbc( Cbc& pCbc ){
		fCbcVector.push_back( pCbc );
	}

	bool Module::removeCbc( uint8_t pCbcId )
	{
		std::vector < Cbc > :: iterator i;
		bool j=false;
		for (i=fCbcVector.begin();i!=fCbcVector.end();++i)
		{
			if (i->fCbcId==pCbcId)
			{
				fCbcVector.erase(i);
				j=true;
				i--;   //erase reduces the container size by the number of elements removed, which are destroyed. To avoid that the iterator point an unallocated part of the memory, we need to decrease the iterator
			}
		}
		if (j==true)
		return true;
		else
		{
			std::cout<<"Error:This Module doesn't have the cbc"<<std::endl;
			return false;
		}
	}

	//Triggers a warning on the referencing
	Cbc*   Module::getCbc( uint8_t pCbcId )
	{
		std::vector < Cbc > :: iterator i;
		for (i=fCbcVector.begin();i!=fCbcVector.end();++i)
		{
			if (i->fCbcId==pCbcId)
				{
					return &*i ;
				}
		}
		return NULL;

	}

}
