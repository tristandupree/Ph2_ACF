#ifndef __EVENT_H__
#define __EVENT_H__

#include <string>
#include <sstream>
#include <cstring>
#include <TROOT.h>
#include <iomanip>
#include "../HWDescription/Definition.h"
#include "../HWDescription/BeBoard.h"

using namespace Ph2_HwDescription;

namespace Ph2_HwInterface
{

	typedef std::map<uint32_t, char*> FeEventMap;
	typedef std::map<uint32_t, FeEventMap> EventMap;

	class Event
    {

		/*
		   id of FeEvent should be the order of FeEvents in data stream starting from 0
		   id of CbcEvent also should be the order of CBCEvents in data stream starting from 0
		 */
        private:
            char *fBuf;
            EventMap fEventMap;
            uint32_t fBunch;
            uint32_t fOrbit;
            uint32_t fLumi;
            uint32_t fEventCount;
            uint32_t fEventCountCBC;
            uint32_t fTDC;

        public:
            Event() {}
			Event(Event &pEvent);
			~Event() {}
			void Clear() { fEventMap.clear(); }
			void AddBoard( BeBoard& pBoard );
			int SetEvent( char *pEvent );

            //user interface
			char* GetCbcEvent( uint8_t& pFeId, uint8_t& pCbcId ) const;
			uint32_t GetBunch() const { return fBunch; }
			uint32_t GetOrbit() const { return fOrbit; }
			uint32_t GetLumi() const { return fLumi; }
			uint32_t GetEventCount() const { return fEventCount; }
			uint32_t GetEventCountCBC() const { return fEventCountCBC; }
			uint32_t GetTDC() const { return fTDC; }
			std::string HexString() const;

			//user interface
			//function to get the bit at the global data string position
			bool Bit(uint8_t pFeId, uint8_t pCbcId, uint32_t pPosition ) const;
			//functions to get Error bit
			bool Error(uint8_t pFeId, uint8_t pCbcId, uint32_t i ) const;
			//functions to get all Error bits
			uint32_t Error(uint8_t pFeId, uint8_t pCbcId) const;
			//functions to get pipeline address
			uint32_t PipelineAddress(uint8_t pFeId, uint8_t pCbcId)const;
			//function to get a CBC pixel bit data
			bool DataBit(uint8_t pFeId, uint8_t pCbcId, uint32_t i)const;
			//function to get bit string from the data offset and width
			std::string BitString(uint8_t pFeId, uint8_t pCbcId, uint32_t pOffset, uint32_t pWidth )const;
			//function to get bit string of CBC data
			std::string DataBitString(uint8_t pFeId, uint8_t pCbcId)const;
			//function to get bit string in hexadecimal format for CBC data
			std::string DataHexString(uint8_t pFeId, uint8_t pCbcId)const;
			//function to get GLIB flag string
			std::string GlibFlagString(uint8_t pFeId, uint8_t pCbcId)const;
			//function to get bit string for CBC STAB data
			std::string StubBitString(uint8_t pFeId, uint8_t pCbcId)const;
			//function to get char at the global data string at position 8*i
			char Char(uint8_t pFeId, uint8_t pCbcId, uint32_t pBytePosition ){ return GetCbcEvent(pFeId, pCbcId)[pBytePosition]; }

	};
}
#endif
