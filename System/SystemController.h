/*!

        \file                    SystemController.h
        \brief                                   Controller of the System, overall wrapper of the framework
        \author                                  Nicolas PIERRE
        \version                 1.0
        \date                                    10/08/14
        Support :                                mail to : lorenzo.bidegain@cern.ch, nico.pierre@icloud.com

 */


#ifndef __SYSTEMCONTROLLER_H__
#define __SYSTEMCONTROLLER_H__

#include "../HWInterface/BeBoardFWInterface.h"
#include "../HWDescription/Shelve.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../Utils/Visitor.h"
#include "../Utils/Utilities.h"

#include "../Utils/pugixml.hpp"
#include "../Utils/ConsoleColor.h"
#include <iostream>
#include <vector>
#include <map>
#include <stdlib.h>
# include <string.h>

#include "TFile.h"


using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

/*!
 * \namespace Ph2_System
 * \brief Namespace regrouping the framework wrapper
 */
namespace Ph2_System
{

	typedef std::vector<Shelve*> ShelveVec;     /*!< Vector of Shelve pointers */
	typedef std::map<std::string, uint32_t> SettingsMap;    /*!< Maps the settings */

	/*!
	 * \class SystemController
	 * \brief Create, initialise, configure a predefined HW structure
	 */
	class SystemController
	{
	  public:
		BeBoardInterface*       fBeBoardInterface;                     /*!< Interface to the BeBoard */
		CbcInterface*           fCbcInterface;                     /*!< Interface to the Cbc */
		ShelveVec fShelveVector;                                           /*!< Vector of Shelve pointers */
		BeBoardFWMap fBeBoardFWMap;                                /*!< Map of connections to the BeBoard */
		SettingsMap fSettingsMap;                                         /*!< Maps the settings */
		std::string fDirectoryName;             /*< the Directoryname for the Root file with results */
		TFile* fResultFile;                /*< the Name for the Root file with results */


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
		 * \brief acceptor method for HwDescriptionVisitor
		 * \param pVisitor
		 */
		void accept( HwDescriptionVisitor& pVisitor ) {
			pVisitor.visit( *this );
			for ( Shelve* cShelve : fShelveVector )
				cShelve->accept( pVisitor );
		}

		// void accept( HwDescriptionVisitor& pVisitor ) const {
		//  pVisitor.visit( *this );
		//  for ( auto& cShelve : fShelveVector )
		//      cShelve->accept( pVisitor );
		// }

		/*!
		 * \brief Create a result directory at the specified path + ChargeMode + Timestamp
		 * \param pDirectoryname : the name of the directory to create
		 * \param pDate : apend the current date and time to the directoryname
		 */
		void CreateResultDirectory( const std::string& pDirectoryname, bool pDate = true );
		/*!
		 * \brief Initialize the result Root file
		 * \param pFilename : Root filename
		 */
		void InitResultFile( const std::string& pFilename );

		/*!
		 * \brief Initialize the Hardware via an XML file
		 * \param pFilename : XML HW Description file
		 */
		void InitializeHw( const std::string& pFilename );
		/*!
		 * \brief Initialize the settings
		 * \param pFilename : XML HW Description file
		 */
		void InitializeSettings( const std::string& pFilename );
		/*!
		 * \brief Configure the Hardware with XML file indicated values
		 */
		void ConfigureHw();
		/*!
		 * \brief Run a DAQ
		 * \param pBeBoard
		 * \param pNthAcq
		 */
		void Run( BeBoard* pBeBoard, uint32_t pNthAcq );

		/*!
		 * \brief converts any char array to int by automatically detecting if it is hex or dec
		 * \param pRegValue: parsed xml parmaeter char*
		 * \return converted integer
		 */
		uint32_t convertAnyInt( const char* pRegValue ) {
			if ( std::string( pRegValue ).find( "0x" ) != std::string::npos ) return static_cast<uint32_t>( strtoul( pRegValue , 0, 16 ) );
			else return static_cast<uint32_t>( strtoul( pRegValue , 0, 10 ) );

		}

	  protected:
		/*!
		 * \convert a voltage level to it's 8bit DAC value
		 * \param pVoltage: the Voltage level
		 * \return corresponding 8-bit DAC value
		 */
		uint32_t Vto8Bit( float pVoltage ) {
			return static_cast<uint32_t>( pVoltage / 3.3 * 256 + 0.5 );
		}
	};
}

#endif
