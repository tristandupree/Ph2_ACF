#ifndef _Glib_h__
#define _Glib_h__

//#include "GlibDefinitions.h"
#include "Module.h"
#include <vector>
#include <map>
#include <boost/cstdint.hpp>


namespace Ph2_HwDescription{

	#define default_glib_file  "glib_settings.cfg"

	// <node id="nb_FE", description="0: 1 FE, 1: 1 FE + TTC FMC, 2: 2 FE"/>
	enum nb_FE { SINGLE_FE, FE_TRG, DUAL_FE };
	typedef std::map< std::string, uint8_t > GlibRegMap;

	// No base class for the Glib uint8_tended yet, maybe this will change
	class Glib{

	public:

		// C'tors: the Glib only needs to know about it's shelf and which BE it is + the # of FEs connected
		// C'tor for a standard Glib
		Glib( uint8_t pShelveId, uint8_t pBeId, uint8_t pNFe, std::string filename = default_glib_file );
		// Parameters that define system for us
		//Modify with new param
		Glib( uint8_t pShelveId, uint8_t pBeId, uint8_t pNFe, uint8_t pFMCConfiguration, bool pExtTrg, bool pFakeData = false , std::string filename = "default_glib_file" );
		// Default C'tor
		Glib();

		// D'tor
		~Glib(){};

		// Public Methods
		uint8_t getNFe(){return fModuleVector.size();};
		uint8_t getBeId(){return fBeId;};
		uint8_t getShelveId(){return fShelveId;};

		uint8_t getReg( std::string pReg );
		void setReg( std::string pReg, uint8_t psetValue );

		void addModule( Module& pModule );
		bool removeModule( uint8_t pModuleId );
		Module& getModule( uint8_t pModuleId );

		std::map< std::string, uint8_t > getGlibRegMap (){return fRegMap;};

	protected:
		// Connection Members
		uint8_t fShelveId;
		uint8_t fBeId;

		// Vector of FEModules, each module is supposed to know which FMC slot it is connected to...
		std::vector< Module > fModuleVector;

		// Map of Glib Register Names vs. Register Values
		GlibRegMap fRegMap;

	private:

		void loadConfigFile( std::string filename );

	};
}

#endif
