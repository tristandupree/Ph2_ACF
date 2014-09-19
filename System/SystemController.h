/*!

	\file					 SystemController.h
	\brief 				 	 Controller of the System, overall wrapper of the framework
	\author 				 Nicolas PIERRE
	\version				 1.0
	\date 				 	 10/08/14
	Support : 				 mail to : lorenzo.bidegain@cern.ch, nico.pierre@icloud.com

*/


#ifndef __SYSTEMCONTROLLER_H__
#define __SYSTEMCONTROLLER_H__

#include "../HWInterface/BeBoardFWInterface.h"
#include "../HWDescription/Shelve.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "pugixml.hpp"
#include "ConsoleColor.h"
#include <iostream>
#include <vector>
#include <map>
#include <stdlib.h>
# include <string.h>


using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

/*!
* \namespace Ph2_System
* \brief Namespace regrouping the framework wrapper
*/
namespace Ph2_System
{

	typedef std::vector<Shelve*> ShelveVec; /*!< Vector of Shelve pointers */
	typedef std::map<std::string,uint8_t> SettingsMap; /*!< Maps the settings */

	/*!
	* \class SystemController
	* \brief Create, initialise, configure a predefined HW structure
	*/
	class SystemController
	{
		public:
			BeBoardInterface*	fBeBoardInterface; /*!< Interface to the BeBoard */
			CbcInterface*		fCbcInterface; /*!< Interface to the Cbc */
			ShelveVec		 	fShelveVec; /*!< Vector of Shelve pointers */
			BeBoardFWMap   		fBeBoardFWMap; /*!< Map of connections to the BeBoard */
			SettingsMap			fSettingsMap; /*!< Maps the settings */

		public:
			/*!
			* \brief Constructor of the SystemController class
			*/
			SystemController();
			/*!
			* \brief Destructor of the SystemController class
			*/
			~SystemController();

			/*!
			* \brief Initialize the Hardware via an XML file
			* \param pFilename : XML HW Description file
			*/
			void InitializeHw(const std::string pFilename);
			/*!
			* \brief Initialize the settings
			* \param pFilename : XML HW Description file
			*/
			void InitializeSettings(const std::string pFilename);
			/*!
			* \brief Configure the Hardware with XML file indicated values
			*/
			void ConfigureHw();
			/*!
			* \brief Run a DAQ
			* \param pBeBoard
			* \param pNthAcq
			*/
			void Run(BeBoard* pBeBoard, uint32_t pNthAcq);
	};
}

#endif
