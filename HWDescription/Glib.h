#ifndef _Glib_h__
#define _Glib_h__

#include "HWDescription/GlibDefinitions.h"
#include "HWDescription/Module.h"

#include <map>
#include <multimap>


namespace Ph2_HwDescription{

	// <node id="nb_FE", description="0: 1 FE, 1: 1 FE + TTC FMC, 2: 2 FE"/>
	enum { SINGLE_FE, FE_TRG, DUAL_FE };
	typedef std::map< std::string, UInt_t > GlibRegMap;

	// No base class for the Glib intended yet, maybe this will change
	class Glib{

	public:

		// C'tors: the Glib only needs to know about it's shelf and which BE it is + the # of FEs connected
		// C'tor for a standard Glib
		Glib( UInt_t pShelveId, UInt_t pBeId, UInt_t pNFe );
		// Parameters that define system for us
		Glib( UInt_t pShelveId, UInt_t pBeId, UInt_t pNFe, UInt_t pHwConfig, bool pExtTrg, bool pFakeData = false , UInt_t pNPackets = 100 );
		// Default C'tor
		Glib();

		// D'tor
		~Glib();

		// Public Methods
		UInt_t getNFe( return fModuleVector.size() );

		.
		.
		.
		std::map< std::string, UInt_t > getGlibRegMap ( return fRegMap );

	protected:

		// Vector of FEModules, each module is supposed to know which FMC slot it is connected to...
		std::vector< Module > fModuleVector;            

		// Some important register Values, not sure if needed, but simpler than accessing by a long and complex register name:

		// FMC configuration, see enum at the top
    	UInt_t     fFMCConfiguration;
		// Internal or external triggers? Enum or #define
		bool       fExtTrg;
		UInt_t     fTrgFreq;
		// bool to signalise if fake CBC data is created internally or not
		bool       fFakeData;
		// Data Size in Packets during Acquisition
		UInt_t     fNPackets; 
		// negative Logic or not depends on the FMC type used
		bool       fNegativeLogicCbc;

		// Map of Glib Register Names vs. Register Values, to be passed to the Glib Interface Class as whole
		GlibRegMap fRegMap;

		// The FW registers that we actually use (not all of them) should be described by the GlibRegMap; However, the parameters above describe the status of the board without having to know the exact register name string
		// Compare address table file. Much of the informagtion in there is contained in the maps
	};
}

#endif