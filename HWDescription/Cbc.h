#ifndef Cbc_h__
#define Cbc_h__

#include "FEDescription.h"
#include "CbcRegItem.h"
#include <iostream>
#include <map>
#include <string>

// Cbc2 Chip HW Description Class

namespace Ph2_HwDescription{

	std::string default_file = "default_file.txt";

	typedef std::map < std::string, CbcRegItem > CbcRegMap; 

	class Cbc : public FEDescription{

	public:

		// C'tors with object FE Description 
		Cbc( FEDescription& pFeDesc, UInt_t pCbcId, std::string filename );
		Cbc( FEDescription& pFeDesc, UInt_t pCbcId,UInt_t pTriggerLatency,UInt_t pVcth );
		Cbc( FEDescription& pFeDesc, UInt_t pCbcId ); 

		// C'tors which take ShelveID, BeId, FeID, CbcId
		Cbc( UInt_t pShelveId, UInt_t pBeId, UInt_t pFMCId, UInt_t pFeId, UInt_t pCbcId, std::string filename );
		Cbc( UInt_t pShelveId, UInt_t pBeId, UInt_t pFMCId, UInt_t pFeId, UInt_t pCbcId, UInt_t pTriggerLatency,UInt_t pVcth );
		Cbc( UInt_t pShelveId, UInt_t pBeId, UInt_t pFMCId, UInt_t pFeId, UInt_t pCbcId );

		// Default C'tor
		Cbc();

		// Copy C'tor
		Cbc(Cbc& cbcobj);

		//load fRegMap from file
		void loadfRegMap(std::string filename);		


		UInt_t getTriggerLatency();
		void setTriggerLatency(UInt_t pTriggerLatency);

		UInt_t getVcth();
		void setVcth(UInt_t psetVcth);

		UInt_t getReg(std::string pReg);
		void setReg(std::string pReg, UInt_t psetValue);

		//Write RegValues in a file
		void writeRegValues( std::string filename );

		UInt_t getCbcId( return fCbcId );
		
	protected:

		UInt_t fCbcId;

		// Map of Register Name vs. RegisterItem that contains: Page, Address, Default Value, Value
		CbcRegMap fRegMap;
		

	private:

	};


	struct CbcComparer{

		bool operator() (const Cbc& cbc1,const Cbc& cbc2);
			
		};

}

#endif
