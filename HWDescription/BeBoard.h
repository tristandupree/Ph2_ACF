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

	// <node id="nb_FE", description="0: 1 FE, 1: 1 FE + TTC FMC, 2: 2 FE"/>
	enum nb_FE { SINGLE_FE, FE_TRG, DUAL_FE };
	typedef std::map< std::string, uint8_t > BeBoardRegMap;

	/*!
	* \class BeBoard
	* \brief Read/Write BeBoard's registers on a file, contains a register map and contains a vector of Module which are connected to the BeBoard
	*/
	class BeBoard{

	public:

		// C'tors: the BeBoard only needs to know about it's shelf and which BE it is + the # of FEs connected
		// C'tor for a standard BeBoard
		BeBoard( uint8_t pShelveId, uint8_t pBeId, uint8_t pNFe, std::string filename);

		// D'tor
		virtual ~BeBoard(){};

		// Public Methods

		/*!
		* \brief Get the number of modules connected to the BeBoard
		* \return The size of the vector
		*/
		virtual uint8_t getNFe(){return fModuleVector.size();};
		/*!
		* \brief Get the Be Id of the BeBoard
		* \return the Be Id
		*/
		virtual uint8_t getBeId(){return fBeId;};
		/*!
		* \brief Get the Shelve Id of the BeBoard
		* \return the Be Id
		*/
		virtual uint8_t getShelveId(){return fShelveId;};

		/*!
		* \brief Get any register from the Map
		* \param pReg
		* \return The value of the register
		*/
		virtual uint8_t getReg( std::string pReg );
		/*!
		* \brief Set any register of the Map
		* \param pReg
		* \param psetValue
		*/
		virtual void setReg( std::string pReg, uint8_t psetValue );

		/*!
		* \brief Adding a module to the vector
		* \param pModule
		*/
		virtual void addModule( Module& pModule );
		/*!
		* \brief Remove a Module from the vector
		* \param pModuleId
		* \return a bool which indicate if the removing was successful
		*/
		virtual bool removeModule( uint8_t pModuleId );
		/*!
		* \brief Get a module from the vector
		* \param pModuleId
		* \return a pointer of module, so we can manipulate directly the module contained in the vector
		*/
		virtual Module* getModule( uint8_t pModuleId );

		virtual std::map< std::string, uint8_t > getBeBoardRegMap (){return fRegMap;};

	protected:
		// Connection Members
		uint8_t fShelveId;
		uint8_t fBeId;

		// Vector of FEModules, each module is supposed to know which FMC slot it is connected to...
		std::vector< Module > fModuleVector;

		// Map of BeBoard Register Names vs. Register Values
		BeBoardRegMap fRegMap;

	private:

		/*!
		* \brief Load RegMap from a file
		* \param filename
		*/
		virtual void loadConfigFile( std::string filename );

	};
}

#endif
