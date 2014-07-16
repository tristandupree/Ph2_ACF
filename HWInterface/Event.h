#ifndef __EVENT_H__
#define __EVENT_H__

#include "FeEvent.h"
#include "CbcEvent.h"
#include "../HWDescription/Definition.h"

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

        private:
            FeEvent *findFeEvent( uint32_t pFE );

        public:
            Event() {}
			Event(Event &pEvent);
			~Event() {}
			void Clear() { fEventMap.clear(); }
			void AddBoard( BeBoard& pBoard );
			int SetEvent( char *pEvent );

            //user interface
			char* GetCbcEvent( uint32_t pFE );
			uint32_t GetBunch() const { return fBunch; }
			uint32_t GetOrbit() const { return fOrbit; }
			uint32_t GetLumi() const { return fLumi; }
			uint32_t GetEventCount() const { return fEventCount; }
			uint32_t GetEventCountCBC() const { return fEventCountCBC; }
			uint32_t GetTDC() const { return fTDC; }
			std::string HexString() const;

			//user interface
			//functions to get Error bit
			bool Error( uint32_t i )const;
			//functions to get all Error bits
			uint32_t Error()const;
			//functions to get pipeline address
			uint32_t PipelineAddress()const;
			//function to get a CBC pixel bit data
			bool DataBit(uint32_t i)const;
			//function to get bit string of CBC data
			std::string DataBitString()const;
			//function to get bit string in hexadecimal format for CBC data
			std::string DataHexString()const;
			//function to get GLIB flag string
			std::string GlibFlagString()const;
			//function to get bit string for CBC STAB data
			std::string StubBitString()const;
			//function to get char at the global data string at position 8*i
			char Char( uint32_t pBytePosition ){ return fBuf[pBytePosition]; }
			//function to get the bit at the global data string position
			bool Bit( uint32_t pPosition )const;

			//function to get bit string from the data offset and width
			std::string BitString( uint32_t pOffset, uint32_t pWidth )const;
			uint32_t Id()const{ return fCbcId; }


	};
}
#endif
