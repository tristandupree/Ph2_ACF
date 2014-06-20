#include "Module.h"

namespace Ph2_HwDescription{

	// Default C'tor
	Module::Module():FEDescription(){}

	Module::Module( FEDescription& pFeDesc):FEDescription(pFeDesc){
	}

	Module::Module( uint32_t pShelveId, uint32_t pBeId, uint32_t pFMCId, uint32_t pFeId ):FEDescription( pShelveId, pBeId, pFMCId,pFeId ){
	}

	void Module::addCbc( Cbc& pCbc ){
		fCbcVector.insert( pCbc );
	}

	bool Module::removeCbc( uint32_t pCbcId )
	{
		std::set < Cbc > :: iterator i;
		bool j=false;
		for (i=fCbcVector.begin();i!=fCbcVector.end();++i)
		{
			if (i->fCbcId==pCbcId)
			{
				fCbcVector.erase(i);
				j=true;
				
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

	Cbc   Module::getCbc( uint32_t pCbcId )
	{
		Cbc obj;
		std::set < Cbc > :: iterator i;
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
