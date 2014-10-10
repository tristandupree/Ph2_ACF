#ifndef __SYSTEMCONTROLLER_H__
#define __SYSTEMCONTROLLER_H__
#include "Model/settings.h"
#include <QVariantMap>

#include "../HWInterface/BeBoardFWInterface.h"
#include "../HWDescription/Shelve.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../HWInterface/Visitor.h"

#include <iostream>
#include <vector>
#include <map>
#include <stdlib.h>
#include <string.h>

#include "TFile.h"


using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

/*!
 * \namespace Ph2_System
 * \brief Namespace regrouping the framework wrapper
 */
namespace GUI
{

    typedef std::vector<Shelve*> ShelveVec;     /*!< Vector of Shelve pointers */
    typedef std::map<std::string, uint32_t> SettingsMap;    /*!< Maps the settings */

    /*!
     * \class SystemController
     * \brief Create, initialise, configure a predefined HW structure
     */
    class SystemControllerWorker
    {
      public:
        BeBoardInterface*       fBeBoardInterface;                     /*!< Interface to the BeBoard */
        CbcInterface*           fCbcInterface;                     /*!< Interface to the Cbc */
        ShelveVec fShelveVector;                                           /*!< Vector of Shelve pointers */
        BeBoardFWMap fBeBoardFWMap;                                /*!< Map of connections to the BeBoard */
        SettingsMap fSettingsMap;                                         /*!< Maps the settings */
        std::string fDirectoryName;             /*< the Directoryname for the Root file with results */



      public:

        SystemControllerWorker(Settings &config);
        /*!
         * \brief Destructor of the SystemController class
         */
        ~SystemControllerWorker();

        /*!
         * \brief acceptor method for HwDescriptionVisitor
         * \param pVisitor
         */

         void accept( HwDescriptionVisitor& pVisitor ) const {
          pVisitor.visit( *this );
          for ( auto& cShelve : fShelveVector )
              cShelve->accept( pVisitor );
         }

        /*!
         * \brief Create a result directory at the specified path + ChargeMode + Timestamp
         * \param pDirectoryname : the name of the directory to create
         */
        void CreateResultDirectory( std::string pDirectoryname );
        /*!
         * \brief Initialize the result Root file
         * \param pFilename : Root filename
         */
        void InitResultFile( std::string pFilename );

        /*!
         * \brief Initialize the Hardware via an XML file
         * \param pFilename : XML HW Description file
         */
        void InitializeHw();
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

      private:
        // uint8_t convertAnyInt( const char* pRegValue ) {
        //  if ( std::string( pRegValue ).find( "0x" ) != std::string::npos ) return uint8_t( strtoul( pRegValue , 0, 16 ) );
        //  else return uint8_t( strtoul( pRegValue , 0, 10 ) );

        // }

        QVariantMap* map_HwDescription;
        Settings& m_Settings;
        uint32_t convertAnyInt( const char* pRegValue ) {

        }
    };
}

#endif
