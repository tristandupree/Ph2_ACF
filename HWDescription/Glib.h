#ifndef _Glib_h__
#define _Glib_h__

//#include "GlibDefinitions.h"
#include "Module.h"
#include <vector>
#include <map>


namespace Ph2_HwDescription{

	#define default_glib_file  "glib_settings.cfg"

	// <node id="nb_FE", description="0: 1 FE, 1: 1 FE + TTC FMC, 2: 2 FE"/>
	enum nb_FE { SINGLE_FE, FE_TRG, DUAL_FE };
	typedef std::map< std::string, UInt_t > GlibRegMap;

	// No base class for the Glib UInt_tended yet, maybe this will change
	class Glib{

	public:

		// C'tors: the Glib only needs to know about it's shelf and which BE it is + the # of FEs connected
		// C'tor for a standard Glib
		Glib( UInt_t pShelveId, UInt_t pBeId, UInt_t pNFe, std::string filename = default_glib_file );
		// Parameters that define system for us
		Glib( UInt_t pShelveId, UInt_t pBeId, UInt_t pNFe, UInt_t pFMCConfiguration, bool pExtTrg, bool pFakeData = false , UInt_t pNPackets = 100,  std::string filename = "default_glib_file" );
		// Default C'tor
		Glib();

		// D'tor
		~Glib(){};

		// Public Methods
		UInt_t getNFe(){return fModuleVector.size();};

		UInt_t getReg( std::string pReg );
		void setReg( std::string pReg, UInt_t psetValue );
		
		void addModule( Module& pModule );
		bool removeModule( UInt_t pModuleId );
		Module getModule( UInt_t pModuleId );

		std::map< std::string, UInt_t > getGlibRegMap (){return fRegMap;};

	protected:
		// Connection Members
		UInt_t fShelveId;
		UInt_t fBeId;

		// Vector of FEModules, each module is supposed to know which FMC slot it is connected to...
		std::vector< Module > fModuleVector;            

		// Some important register Values, not sure if needed, but simpler than accessing by a long and complex register name:

		// FMC configuration, see enum at the top
    		UInt_t     fFMCConfiguration;
		// UInt_ternal or external triggers? Enum or #define
		bool       fExtTrg;
		UInt_t     fTrgFreq;
		// bool to signalise if fake CBC data is created UInt_ternally or not
		bool       fFakeData;
		// Data Size in Packets during Acquisition
		UInt_t     fNPackets; 
		// negative Logic or not depends on the FMC type used
		bool       fNegativeLogicCbc;

		// Map of Glib Register Names vs. Register Values
		GlibRegMap fRegMap;

	private:

		void loadConfigFile( std::string filename );

	};
}

#endif
