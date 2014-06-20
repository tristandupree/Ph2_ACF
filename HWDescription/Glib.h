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
	typedef std::map< std::string, uint32_t > GlibRegMap;

	// No base class for the Glib uint32_tended yet, maybe this will change
	class Glib{

	public:

		// C'tors: the Glib only needs to know about it's shelf and which BE it is + the # of FEs connected
		// C'tor for a standard Glib
		Glib( uint32_t pShelveId, uint32_t pBeId, uint32_t pNFe, std::string filename = default_glib_file );
		// Parameters that define system for us
		Glib( uint32_t pShelveId, uint32_t pBeId, uint32_t pNFe, uint32_t pFMCConfiguration, bool pExtTrg, bool pFakeData = false , std::string filename = "default_glib_file" );
		// Default C'tor
		Glib();

		// D'tor
		~Glib(){};

		// Public Methods
		uint32_t getNFe(){return fModuleVector.size();};
		uint32_t getBeId(){return fBeId;};

		uint32_t getReg( std::string pReg );
		void setReg( std::string pReg, uint32_t psetValue );
		
		void addModule( Module& pModule );
		bool removeModule( uint32_t pModuleId );
		Module getModule( uint32_t pModuleId );

		std::map< std::string, uint32_t > getGlibRegMap (){return fRegMap;};

	protected:
		// Connection Members
		uint32_t fShelveId;
		uint32_t fBeId;

		// Vector of FEModules, each module is supposed to know which FMC slot it is connected to...
		std::vector< Module > fModuleVector;            

		// Some important register Values, not sure if needed, but simpler than accessing by a long and complex register name:

		// FMC configuration, see enum at the top
    		uint32_t     fFMCConfiguration;
		// uint32_ternal or external triggers? Enum or #define
		bool       fExtTrg;
		uint32_t     fTrgFreq;
		// bool to signalise if fake CBC data is created uint32_ternally or not
		bool       fFakeData;
		// Data Size in Packets during Acquisition
		//uint32_t     fNPackets; 
		// negative Logic or not depends on the FMC type used
		//bool       fNegativeLogicCbc;

		// Map of Glib Register Names vs. Register Values
		GlibRegMap fRegMap;

	private:

		void loadConfigFile( std::string filename );

	};
}

#endif
