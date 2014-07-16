#ifndef __CBCEVENT_H__
#define __CBCEVENT_H__

#include <string>
#include <sstream>
#include <cstring>
#include <TROOT.h>

namespace Ph2_HwInterface
{

	class CbcEvent
    {

        private:
            uint32_t fFeId;
            uint32_t fCbcId;
            char *fBuf;

        /*
		single CBC event
		The Id should corresponds to the order of in the stream data starting from 0
		 */
		public:
			static const uint32_t NCHAR = 9*4;
			static const uint32_t NSENSOR;

			static const uint32_t OFFSET_ERROR;
			static const uint32_t WIDTH_ERROR;
			static const uint32_t OFFSET_PIPELINE_ADDRESS;
			static const uint32_t WIDTH_PIPELINE_ADDRESS;
			static const uint32_t OFFSET_CBCDATA;
			static const uint32_t WIDTH_CBCDATA;
			static const uint32_t OFFSET_GLIBFLAG;
			static const uint32_t WIDTH_GLIBFLAG;
			static const uint32_t OFFSET_CBCSTABDATA;
			static const uint32_t WIDTH_CBCSTABDATA;

			CbcEvent(uint32_t pFeId, uint32_t pId=0);
			CbcEvent(const CbcEvent& pEvent);
			~CbcEvent(){}


			void Set( char *pEvent );

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
