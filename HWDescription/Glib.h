/*!
*
* \file Glib.h
* \brief Glib Description class, configs of the Glib
* \author Lorenzo BIDEGAIN
* \date 25/06/14
*
* Support : mail to : lorenzo.bidegain@cern.ch
*
*/

#ifndef _Glib_h__
#define _Glib_h__

#include "Definition.h"
#include "Module.h"
#include <vector>
#include <map>
#include <boost/cstdint.hpp>

/*!
* \namespace Ph2_HwDescription
* \brief Namespace regrouping all the hardware description
*/
namespace Ph2_HwDescription{

	#define default_glib_file  "glib_settings.cfg"

	// <node id="nb_FE", description="0: 1 FE, 1: 1 FE + TTC FMC, 2: 2 FE"/>
	enum nb_FE { SINGLE_FE, FE_TRG, DUAL_FE };
	typedef std::map< std::string, uint8_t > GlibRegMap;

	// No base class for the Glib intended yet, maybe this will change

	/*!
	* \class Glib
	* \brief Read/Write Glib's registers on a file, contains a register map and contains a vector of Module which are connected to the Glib 
	*/
	class Glib{

	public:

		// C'tors: the Glib only needs to know about it's shelf and which BE it is + the # of FEs connected
		// C'tor for a standard Glib
		Glib( uint8_t pShelveId, uint8_t pBeId, uint8_t pNFe, std::string filename = default_glib_file );
		// Parameters that define system for us
		//Modify with new param
		Glib( uint8_t pShelveId, uint8_t pBeId, uint8_t pNFe,/* uint8_t pFMCConfiguration,*/ bool pExtTrg, bool pFakeData = false , std::string filename = "default_glib_file" );
		// Default C'tor
		Glib();

		// D'tor
		~Glib(){};

		// Public Methods

		/*!
		* \brief Get the number of modules connected to the Glib
		* \return The size of the vector
		*/
		uint8_t getNFe(){return fModuleVector.size();};
		/*!
		* \brief Get the Be Id of the Module
		* \return the Be Id
		*/
		uint8_t getBeId(){return fBeId;};
		/*!
		* \brief Get the Shelve Id of the Module
		* \return the Be Id
		*/
		uint8_t getShelveId(){return fShelveId;};

		/*!
		* \brief Get any register from the Map
		* \param pReg
		* \return The value of the register
		*/
		uint8_t getReg( std::string pReg );
		/*!
		* \brief Set any register of the Map
		* \param pReg
		* \param psetValue
		*/
		void setReg( std::string pReg, uint8_t psetValue );

		/*!
		* \brief Adding a module to the vector
		* \param pModule
		*/
		void addModule( Module& pModule );
		/*!
		* \brief Remove a Module from the vector
		* \param pModuleId
		* \return a bool which indicate if the removing was successful
		*/
		bool removeModule( uint8_t pModuleId );
		/*!
		* \brief Get a module from the vector
		* \param pModuleId
		* \return a pointer of module, so we can manipulate directly the module contained in the vector
		*/
		Module* getModule( uint8_t pModuleId );

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

		/*!
		* \brief Load RegMap from a file
		* \param filename
		*/
		void loadConfigFile( std::string filename );

	};
}

#endif
