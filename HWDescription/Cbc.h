#ifndef Cbc_h__
#define Cbc_h__

#include "FEDescription.h"
#include "CbcRegItem.h"
#include <iostream>
#include <map>
#include <string>
#include <boost/cstdint.hpp>

// Cbc2 Chip HW Description Class

namespace Ph2_HwDescription{

	#define default_file "default_file.txt"
	typedef std::map < std::string, CbcRegItem > CbcRegMap;

	class Cbc : public FEDescription{

	public:

		// C'tors with object FE Description
		Cbc( FEDescription& pFeDesc, uint8_t pCbcId, std::string filename );
		Cbc( FEDescription& pFeDesc, uint8_t pCbcId,uint8_t pTriggerLatency,uint8_t pVcth );
		Cbc( FEDescription& pFeDesc, uint8_t pCbcId );

		// C'tors which take ShelveID, BeId, FeID, CbcId
		Cbc( uint8_t pShelveId, uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId, uint8_t pCbcId, std::string filename );
		Cbc( uint8_t pShelveId, uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId, uint8_t pCbcId, uint8_t pTriggerLatency,uint8_t pVcth );
		Cbc( uint8_t pShelveId, uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId, uint8_t pCbcId );

		// Default C'tor
		Cbc();

		// Copy C'tor
		Cbc(const Cbc& cbcobj);

		// D'Tor
		~Cbc();

		//load fRegMap from file
		void loadfRegMap(std::string filename);


		uint8_t getTriggerLatency();
		void setTriggerLatency(uint8_t pTriggerLatency);

		uint8_t getVcth();
		void setVcth(uint8_t psetVcth);

		uint8_t getReg(std::string pReg);
		void setReg(std::string pReg, uint8_t psetValue);

		//Write RegValues in a file
		void writeRegValues( std::string filename );

		uint8_t getCbcId() {return fCbcId;};

		CbcRegMap getRegMap() {return fRegMap;};

	protected:

		// Map of Register Name vs. RegisterItem that contains: Page, Address, Default Value, Value
		CbcRegMap fRegMap;

	public:

		uint8_t fCbcId;

	};



	struct CbcComparer{

		bool operator() (const Cbc& cbc1,const Cbc& cbc2);

		};

}

#endif
