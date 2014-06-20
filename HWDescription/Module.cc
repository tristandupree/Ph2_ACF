#include "Module.h"

namespace Ph2_HwDescription{

	// Default C'tor
	Module::Module():FEDescription(){}

	Module::Module( FEDescription& pFeDesc):FEDescription(pFeDesc){
	}

	Module::Module( uint8_t pShelveId, uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId ):FEDescription( pShelveId, pBeId, pFMCId,pFeId ){
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

	Cbc&   Module::getCbc( uint8_t pCbcId )
	{
		Cbc obj;
		std::vector < Cbc > :: iterator i;
		for (i=fCbcVector.begin();i!=fCbcVector.end();++i)
		{
			if (i->fCbcId==pCbcId)
				{
					return *i;
				}
		}
		return obj;	

	}

}
