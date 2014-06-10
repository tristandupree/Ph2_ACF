#ifndef _Glib_h__
#define _Glib_h__

#include "HWDescription/GlibDefinitions.h"
#include "HWDescription/GlibRegItem.h"
#include "HWDescription/Module.h"

#include <multimap>


namespace Ph2_HwDescription{

	typedef std::map< std::string, GlibRegItem > GlibRegMap;
	typedef std::multimap< UInt_t, FEDescription > FMCMap;

	// No base class for the Glib intended yet, maybe this will change
	class Glib{

	public:

		// C'tors: the Glib only needs to know about it's shelf and which BE it is + the # of FEs connected

		// Register Values that are always the same are to be found in GlibDefinitions.h

		Glib( UInt_t pShelveId, UInt_t pBeId, UInt_t pNFe );
		// Parameters should be # of Packets, Trg Mode, ...?
		Glib( UInt_t pShelveId, UInt_t pBeId, UInt_t pNFe /*, Parameters */);

		// Default C'tor
		Glib();

		// D'tor
		~Glib();


		// Public Methods
		UInt_t getNFe( return fNFe );
		// Methods, Methods & more Methods
		// To evolve with time!
		.
		.
		.
		.

	protected:

		// The number of FEs connected to the Glib
		UInt_t     fNFe;              
		// The mapping of FMC slot to FE! Several FE's will be able to share an FMC slot in the future, that is why it is a multimap
		FMCMap     fFMCMap;                   
		// Map of Glib Register Names vs. Register Values, to be passed to the Glib Interface Class as whole

		GlibRegMap fRegMap;

		// Some important register Values, not sure if needed, but simpler than accessing by a long and complex register name:

		// Internal or external triggers? Enum or #define
		UInt_t     fTrgMode;
		UInt_T     fTrgFreq;

		// stub data latency for all < FEs, Latency > 
		std::multimap< UInt_t, UInt_t > fStubLatency;

		// bool to signalise if fake CBC data is created internally or not
		bool       fInternalData;

		// Data Size in Packets during Acquisition
		UInt_t     fNPackets; 

		// negative Logic or not depends on the FMC type used
		bool       fNegativeLogicCbc;

		// The FW registers that we actually use (not all of them) should be described by the GlibRegMap; However, the parameters above describe the status of the board without having to know the exact register name string
		// Compare address table file. Much of the informagtion in there is contained in the maps
	};
}

#endif