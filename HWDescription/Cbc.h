#ifndef Cbc_h__
#define Cbc_h__

#include "FEDescription.h"
#include "CbcRegItem.h"
#include <iostream>
#include <map>
#include <string>

// Cbc2 Chip HW Description Class

namespace Ph2_HwDescription{

	#define default_file "default_file.txt"
	typedef std::map < std::string, CbcRegItem > CbcRegMap;

	class Cbc : public FEDescription{

	public:

		// C'tors with object FE Description
		Cbc( FEDescription& pFeDesc, uint32_t pCbcId, std::string filename );
		Cbc( FEDescription& pFeDesc, uint32_t pCbcId,uint32_t pTriggerLatency,uint32_t pVcth );
		Cbc( FEDescription& pFeDesc, uint32_t pCbcId );

		// C'tors which take ShelveID, BeId, FeID, CbcId
		Cbc( uint32_t pShelveId, uint32_t pBeId, uint32_t pFMCId, uint32_t pFeId, uint32_t pCbcId, std::string filename );
		Cbc( uint32_t pShelveId, uint32_t pBeId, uint32_t pFMCId, uint32_t pFeId, uint32_t pCbcId, uint32_t pTriggerLatency,uint32_t pVcth );
		Cbc( uint32_t pShelveId, uint32_t pBeId, uint32_t pFMCId, uint32_t pFeId, uint32_t pCbcId );

		// Default C'tor
		Cbc();

		// Copy C'tor
		Cbc(const Cbc& cbcobj);

		// D'Tor
		~Cbc();

		//load fRegMap from file
		void loadfRegMap(std::string filename);


		uint32_t getTriggerLatency();
		void setTriggerLatency(uint32_t pTriggerLatency);

		uint32_t getVcth();
		void setVcth(uint32_t psetVcth);

		uint32_t getReg(std::string pReg);
		void setReg(std::string pReg, uint32_t psetValue);

		//Write RegValues in a file
		void writeRegValues( std::string filename );

		uint32_t getCbcId() {return fCbcId;};

	protected:

		// Map of Register Name vs. RegisterItem that contains: Page, Address, Default Value, Value
		CbcRegMap fRegMap;

	public:

		uint32_t fCbcId;

	};



	struct CbcComparer{

		bool operator() (const Cbc& cbc1,const Cbc& cbc2);

		};

}

#endif
