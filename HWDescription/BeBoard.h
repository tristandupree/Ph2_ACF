/*!
*
* \file BeBoard.h
* \brief BeBoard Description class, configs of the BeBoard
* \author Lorenzo BIDEGAIN
* \date 14/07/14
*
* Support : mail to : lorenzo.bidegain@cern.ch
*
*/

#ifndef _BeBoard_h__
#define _BeBoard_h__

#include "Definition.h"
#include "Module.h"
#include <vector>
#include <map>
#include <boost/cstdint.hpp>
#include "Definition.h"

/*!
* \namespace Ph2_HwDescription
* \brief Namespace regrouping all the hardware description
*/
namespace Ph2_HwDescription{

	typedef std::map< std::string, uint16_t > BeBoardRegMap;

	/*!
	* \class BeBoard
	* \brief Read/Write BeBoard's registers on a file, contains a register map and contains a vector of Module which are connected to the BeBoard
	*/
	class BeBoard{

	public:

		// C'tors: the BeBoard only needs to know about it's shelf and which BE it is + the # of FEs connected
		// Default C'tor
		BeBoard();
		// C'tor for a standard BeBoard
		BeBoard( uint8_t pShelveId, uint8_t pBeId, uint8_t pNFe, std::string filename = DEFAULT_GLIB_FILE );
		// Parameters that define system for us
		//Modify with new param
		BeBoard( uint8_t pShelveId, uint8_t pBeId, uint8_t pNFe,/* uint8_t pFMCConfiguration,*/ bool pExtTrg, bool pFakeData = false , std::string filename = DEFAULT_GLIB_FILE );
		// D'tor
		~BeBoard(){};

		// Public Methods

		/*!
		* \brief Get the number of modules connected to the BeBoard
		* \return The size of the vector
		*/
		uint8_t getNFe(){return fModuleVector.size();};
		/*!
		* \brief Get the Be Id of the BeBoard
		* \return the Be Id
		*/
		uint8_t getBeId(){return fBeId;};
		/*!
		* \brief Get the Shelve Id of the BeBoard
		* \return the Be Id
		*/
		uint8_t getShelveId(){return fShelveId;};

		/*!
		* \brief Get any register from the Map
		* \param pReg
		* \return The value of the register
		*/
		uint16_t getReg( std::string pReg );
		/*!
		* \brief Set any register of the Map
		* \param pReg
		* \param psetValue
		*/
		void setReg( std::string pReg, uint16_t psetValue );

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

		std::map< std::string, uint16_t > getBeBoardRegMap (){return fRegMap;};

	public:
		//Connection Members
		uint8_t fBeId;
		uint8_t fShelveId;

	protected:
		// Vector of FEModules, each module is supposed to know which FMC slot it is connected to...
		std::vector< Module > fModuleVector;

		// Map of BeBoard Register Names vs. Register Values
		BeBoardRegMap fRegMap;

	private:

		/*!
		* \brief Load RegMap from a file
		* \param filename
		*/
		void loadConfigFile( std::string filename );

	};
}

#endif
