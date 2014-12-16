/*

        FileName :                    SystemController.cc
        Content :                     Controller of the System, overall wrapper of the framework
        Programmer :                  Nicolas PIERRE
        Version :                     1.0
        Date of creation :            10/08/14
        Support :                     mail to : nicolas.pierre@cern.ch

 */

#include "SystemController.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

namespace Ph2_System
{

	SystemController::SystemController()
	{

	}

	SystemController::~SystemController()
	{
		fShelveVector.clear();
	}

	void SystemController::InitializeHw( const std::string& pFilename )
	{

		pugi::xml_document doc;
		uint32_t cShelveId, cBeId, cModuleId, cCbcId;
		uint32_t cNShelve = 0;
		int i, j;

		pugi::xml_parse_result result = doc.load_file( pFilename.c_str() );

		if ( !result )
		{
			std::cout << "ERROR :\n Unable to open the file : " << pFilename << std::endl;
			std::cout << "Error description : " << result.description() << std::endl;
			return;
		}

		std::cout << "\n\n\n";
		for ( i = 0; i < 80; i++ )
			std::cout << "*";
		std::cout << "\n";
		for ( j = 0; j < 40; j++ )
			std::cout << " ";
		std::cout << BOLDRED << "HW SUMMARY: " << RESET << std::endl;
		for ( i = 0; i < 80; i++ )
			std::cout << "*";
		std::cout << "\n";
		std::cout << "\n";

		// Iterate the Shelve Nodes
		for ( pugi::xml_node cShelveNode = doc.child( "HwDescription" ).child( "Shelve" ); cShelveNode; cShelveNode = cShelveNode.next_sibling() )
		{
			cShelveId = cShelveNode.attribute( "Id" ).as_int();
			fShelveVector.push_back( new Shelve( cShelveId ) );

			std::cout << BOLDCYAN << cShelveNode.name() << "  " << cShelveNode.first_attribute().name() << " :" << cShelveNode.attribute( "Id" ).value() << RESET << std:: endl;

			// Iterate the BeBoard Node
			for ( pugi::xml_node cBeBoardNode = cShelveNode.child( "BeBoard" ); cBeBoardNode; cBeBoardNode = cBeBoardNode.next_sibling() )
			{

				std::cout << BOLDCYAN << "|" << "----" << cBeBoardNode.name() << "  " << cBeBoardNode.first_attribute().name() << " :" << cBeBoardNode.attribute( "Id" ).value() << RESET << std:: endl;

				cBeId = cBeBoardNode.attribute( "Id" ).as_int();
				BeBoard* cBeBoard = new BeBoard( cShelveId, cBeId );

				// Iterate the BeBoardRegister Nodes
				for ( pugi::xml_node cBeBoardRegNode = cBeBoardNode.child( "Register" ); cBeBoardRegNode != cBeBoardNode.child( "Module" ); cBeBoardRegNode = cBeBoardRegNode.next_sibling() )
				{
					// std::cout << BOLDCYAN << "|" << "  " << "|" << "_____" << cBeBoardRegNode.name() << "  " << cBeBoardRegNode.first_attribute().name() << " :" << cBeBoardRegNode.attribute( "name" ).value() << RESET << std:: endl;
					cBeBoard->setReg( std::string( cBeBoardRegNode.attribute( "name" ).value() ), atoi( cBeBoardRegNode.first_child().value() ) );
				}

				fShelveVector[cNShelve]->addBoard( cBeBoard );

				BeBoardFWInterface* cBeBoardFWInterface;

				if ( std::string( cBeBoardNode.attribute( "boardType" ).value() ).compare( std::string( "Glib" ) ) )
				{
					cBeBoardFWInterface = new GlibFWInterface( doc.child( "HwDescription" ).child( "Connections" ).attribute( "name" ).value(), cBeId );
					fBeBoardFWMap[cBeBoard->getBeBoardIdentifier()] = cBeBoardFWInterface;
				}
				/*else
				        cBeBoardFWInterface = new OtherFWInterface();*/

				// Iterate the module node
				for ( pugi::xml_node cModuleNode = cBeBoardNode.child( "Module" ); cModuleNode; cModuleNode = cModuleNode.next_sibling() )
				{
					if ( static_cast<std::string>( cModuleNode.name() ) == "Module" )
					{
						std::cout << BOLDCYAN << "|" << "	" << "|" << "----" << cModuleNode.name() << "  " << cModuleNode.first_attribute().name() << " :" << cModuleNode.attribute( "ModuleId" ).value() << RESET << std:: endl;

						cModuleId = cModuleNode.attribute( "ModuleId" ).as_int();

						Module* cModule = new Module( cShelveId, cBeId, cModuleNode.attribute( "FMCId" ).as_int(), cModuleNode.attribute( "FeId" ).as_int(), cModuleId );
						fShelveVector[cNShelve]->getBoard( cBeId )->addModule( cModule );

						pugi::xml_node cCbcPathPrefixNode = cModuleNode.child( "CBC_Files" );
						std::string cFilePrefix = std::string( cCbcPathPrefixNode.attribute( "path" ).value() );
						if ( !cFilePrefix.empty() ) std::cout << GREEN << "|" << "	" << "|" << "	" << "|" << "----" << "CBC Files Path : " << cFilePrefix << RESET << std::endl;

						// Iterate the CBC node
						for ( pugi::xml_node cCbcNode = cModuleNode.child( "CBC" ); cCbcNode; cCbcNode = cCbcNode.next_sibling() )
						{
							std::cout << BOLDCYAN << "|" << "	" << "|" << "	" << "|" << "----" << cCbcNode.name() << "  " << cCbcNode.first_attribute().name() << " :" << cCbcNode.attribute( "Id" ).value() << RESET << std:: endl;


							std::string cFileName;
							if ( !cFilePrefix.empty() )
								cFileName = cFilePrefix + cCbcNode.attribute( "configfile" ).value();
							else cFileName = cCbcNode.attribute( "configfile" ).value();

							Cbc* cCbc = new Cbc( cShelveId, cBeId, cModuleNode.attribute( "FMCId" ).as_int(), cModuleNode.attribute( "FeId" ).as_int(), cCbcNode.attribute( "Id" ).as_int(), cFileName );

							for ( pugi::xml_node cCbcRegisterNode = cCbcNode.child( "Register" ); cCbcRegisterNode; cCbcRegisterNode = cCbcRegisterNode.next_sibling() )
								cCbc->setReg( std::string( cCbcRegisterNode.attribute( "name" ).value() ), atoi( cCbcRegisterNode.first_child().value() ) );

							for ( pugi::xml_node cCbcGlobalNode = cModuleNode.child( "Global_CBC_Register" ); cCbcGlobalNode != cModuleNode.child( "CBC" ) && cCbcGlobalNode != cModuleNode.child( "CBC_Files" ) && cCbcGlobalNode != nullptr; cCbcGlobalNode = cCbcGlobalNode.next_sibling() )
							{

								if ( cCbcGlobalNode != nullptr )
								{
									std::string regname = std::string( cCbcGlobalNode.attribute( "name" ).value() );
									uint32_t regvalue = convertAnyInt( cCbcGlobalNode.first_child().value() ) ;
									cCbc->setReg( regname, uint8_t( regvalue ) ) ;

									std::cout << GREEN << "|" << "	" << "|" << "	" << "|" << "----" << cCbcGlobalNode.name() << "  " << cCbcGlobalNode.first_attribute().name() << " :" << regname << " =  0x" << std::hex << std::setw( 2 ) << std::setfill( '0' ) << regvalue << std::dec << RESET << std:: endl;
								}
							}
							fShelveVector[cNShelve]->getBoard( cBeId )->getModule( cModuleId )->addCbc( cCbc );
						}
					}
				}

			}

			cNShelve++;
		}

		fBeBoardInterface = new BeBoardInterface( fBeBoardFWMap );
		fCbcInterface = new CbcInterface( fBeBoardFWMap );
		std::cout << "\n";
		std::cout << "\n";
		for ( i = 0; i < 80; i++ )
			std::cout << "*";
		std::cout << "\n";
		for ( j = 0; j < 40; j++ )
			std::cout << " ";
		std::cout << BOLDRED << "END OF HW SUMMARY: " << RESET << std::endl;
		for ( i = 0; i < 80; i++ )
			std::cout << "*";
		std::cout << "\n";
		std::cout << "\n";
	}

	void SystemController::InitializeSettings( const std::string& pFilename )
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file( pFilename.c_str() );

		if ( !result )
		{
			std::cout << "ERROR :\n Unable to open the file : " << pFilename << std::endl;
			std::cout << "Error description : " << result.description() << std::endl;
			return;
		}
		for ( pugi::xml_node nSettings = doc.child( "Settings" ); nSettings; nSettings = nSettings.next_sibling() )
		{
			for ( pugi::xml_node nSetting = nSettings.child( "Setting" ); nSetting; nSetting = nSetting.next_sibling() )
			{
				fSettingsMap[nSetting.attribute( "name" ).value()] = convertAnyInt( nSetting.first_child().value() );
				std:: cout << RED << "Setting" << RESET << " --" << BOLDCYAN << nSetting.attribute( "name" ).value() << RESET << ":" << BOLDYELLOW << convertAnyInt( nSetting.first_child().value() ) << RESET << std:: endl;
			}
		}
	}

	void SystemController::ConfigureHw()
	{

		bool cHoleMode, cCheck;
		if ( !fSettingsMap.empty() )
		{
			SettingsMap::iterator cSetting = fSettingsMap.find( "HoleMode" );
			if ( cSetting != fSettingsMap.end() )
			{
				cHoleMode = cSetting->second;
				std::cout << GREEN << "Overriding GLIB register values for signal polarity with value from settings node!" << RESET << std::endl;
			}
			cCheck = true;
		}
		else cCheck = false;

		class Configurator : public HwDescriptionVisitor
		{
		  private:
			bool fHoleMode, fCheck;
			Ph2_HwInterface::BeBoardInterface* fBeBoardInterface;
			Ph2_HwInterface::CbcInterface* fCbcInterface;
		  public:
			Configurator( Ph2_HwInterface::BeBoardInterface* pBeBoardInterface, Ph2_HwInterface::CbcInterface* pCbcInterface, bool pHoleMode, bool pCheck ): fBeBoardInterface( pBeBoardInterface ), fCbcInterface( pCbcInterface ), fHoleMode( pHoleMode ), fCheck( pCheck ) {}

			void visit( BeBoard& pBoard ) {
				fBeBoardInterface->ConfigureBoard( &pBoard );

				if ( fCheck )
					fBeBoardInterface->WriteBoardReg( &pBoard, NEG_LOGIC_CBC, ( ( fHoleMode ) ? 0 : 1 ) );
				std::cout << GREEN << "Successfully configured Board " << int( pBoard.getBeId() ) << RESET << std::endl;
			}

			void visit( Cbc& pCbc ) {
				fCbcInterface->ConfigureCbc( &pCbc );
				std::cout << GREEN <<  "Successfully configured Cbc " << int( pCbc.getCbcId() ) << RESET << std::endl;

			}
		};

		Configurator cConfigurator( fBeBoardInterface, fCbcInterface, cHoleMode, cCheck );
		accept( cConfigurator );
	}

	void SystemController::CreateResultDirectory( const std::string& pDirname, bool pDate )
	{
		bool cCheck;
		bool cHoleMode;
		auto cSetting = fSettingsMap.find( "HoleMode" );
		if ( cSetting != std::end( fSettingsMap ) )
		{
			cCheck = true;
			cHoleMode = ( cSetting->second == 1 ) ? true : false;
		}
		std::string cMode;
		if ( cCheck )
		{
			if ( cHoleMode ) cMode = "_Hole";
			else cMode = "_Electron";
		}

		std::string nDirname = pDirname;
		if ( cCheck ) nDirname +=  cMode;
		if ( pDate ) nDirname +=  currentDateTime();
		std::cout << std::endl << "Creating directory: " << nDirname << std::endl << std::endl;
		std::string cCommand = "mkdir -p " + nDirname;

		system( cCommand.c_str() );

		fDirectoryName = nDirname;
	}

	void SystemController::InitResultFile( const std::string& pFilename )
	{

		if ( !fDirectoryName.empty() )
		{
			std::string cFilename = fDirectoryName + "/" + pFilename + ".root";
			fResultFile = TFile::Open( cFilename.c_str(), "RECREATE" );
		}
		else std::cout << RED << "ERROR: " << RESET << "No Result Directory initialized - not saving results!" << std::endl;
	}

	void SystemController::Run( BeBoard* pBeBoard, uint32_t pNthAcq )
	{
		fBeBoardInterface->Start( pBeBoard );
		fBeBoardInterface->ReadData( pBeBoard, pNthAcq, true );
		fBeBoardInterface->Stop( pBeBoard, pNthAcq );
	}
}
