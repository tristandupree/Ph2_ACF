#ifndef __FEEVENT_H__
#define __FEEVENT_H__
#include "CbcEvent.h"
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include <TROOT.h>

namespace Ph2_HwInterface
{

	class FeEvent
    {

		/*
		single FE event
		The Id should corresponds to the order of in the stream data starting from 0
		*/

        private:
            uint32_t fFeId;
            char *fBuf;
            std::vector<CbcEvent> fCbcEvents;

        public:
            static const uint32_t NCHAR = CbcEvent::NCHAR * 2;

		public:
			FeEvent( uint32_t pId) { fFeId = pId; }
			FeEvent( const FeEvent &pE ):fFeId(pE.fFeId),fBuf(0), fCbcEvents(pE.fCbcEvents) {}
			~FeEvent(){}

			void AddCbc( uint32_t pCBC );
			void SetEvent( char *pEvent );

			//user interface
			const CbcEvent *GetCbcEvent( uint32_t pCBC ) const;
			std::string HexString();
			uint32_t Id() const { return fFeId; }

	};

    typedef std::pair<uint32_t, FeEvent> FeEventIdPair;
    typedef std::map<uint32_t, FeEvent> FeEventMap;

}
#endif
