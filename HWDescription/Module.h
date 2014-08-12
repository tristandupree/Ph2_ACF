/*!
*
* \file Module.h
* \brief Module Description class
* \author Lorenzo BIDEGAIN
* \date 25/06/14
*
* Support : mail to : lorenzo.bidegain@cern.ch
*
*/

#ifndef Module_h__
#define Module_h__

#include "FrontEndDescription.h"
#include "Cbc.h"
#include <vector>
#include <boost/cstdint.hpp>

// FE Hybrid HW Description Class

/*!
* \namespace Ph2_HwDescription
* \brief Namespace regrouping all the hardware description
*/
namespace Ph2_HwDescription{

	/*!
	* \class Module
	* \brief contains a vector of Cbc which are connected to the Module
	*/
	class Module : public FrontEndDescription{

	public:

		// C'tors take FrontEndDescription or hierachy of connection and the # of CBCs
		Module( FrontEndDescription& pFeDesc, uint8_t pModuleId);
		Module ( uint8_t pShelveId, uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId, uint8_t pModuleId);

		// Default C'tor
		Module();

		// D'tor
		~Module(){};

		/*!
		* \brief Get the number of Cbc connected to the Module
		* \return The size of the vector
		*/
		uint8_t getNCbc(){return fCbcVector.size();};
		/*!
		* \brief Adding a Cbc to the vector
		* \param pCbc
		*/
		void addCbc( Cbc& pCbc );
		/*!
		* \brief Remove a Cbc from the vector
		* \param pCbcId
		* \return a bool which indicate if the removing was successful
		*/
		bool   removeCbc( uint8_t pCbcId );
		/*!
		* \brief Get a Cbc from the vector
		* \param pCbcId
		* \return a pointer of Cbc, so we can manipulate directly the Cbc contained in the vector
		*/
		Cbc*   getCbc( uint8_t pCbcId );

		//moduleID
		uint8_t fModuleId;

	protected:

		std::vector < Cbc > fCbcVector;

		// This is really all this class needs at the moment, connection and status are already included in the FrontEndDescription parent class!

	};
}


#endif
