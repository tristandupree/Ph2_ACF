#include "Glib.h"

namespace Ph2_HwDescription{

	// Constructors
	Glib::Glib():fShelveId( 0 ), fBeId( 0 ){

		loadConfigFile( default_glib_file );

	}

	Glib::Glib( UInt_t pShelveId, UInt_t pBeId, UInt_t pNFe, std::string filename ):fShelveId( pShelveId ),fBeId( pBeId ){

		fModuleVector.reserve(pNFe);
		loadConfigFile( filename );

	}

	// Public Members:
	UInt_t Glib::getReg( std:string pReg ){
		GlibRegMap::iterator i;
		i = fRegMap.find( pReg );
		if( i = fRegMap.end() ) std::cout << "Register " << pReg << " not found in Glib Register Map!" << std::endl;
		else return i->second;
	}

	UInt_t Glib::setReg( std::string pReg, UInt_t psetValue ){
		GlibRegMap::iterator i;
		i = fRegMap.find( pReg );
		if( i = fRegMap.end() ) std::cout << "Register " << pReg << " not found in Glib Register Map!" << std::endl;
		else fRegMap[pReg] = psetValue;
	}

	// Private Members:

	Glib::loadConfigFile( std::string filename ){

		std::ifstream cFile( filename.c_str(),std::ios::in );
		if( ! cFile ) std::cerr << "The Glib Settings File " << filename << " could not be opened!" << std::endl;
		else{

			std::cout << "Reading Glib settings file " << filename << std::endl;

			fRegMap.clear();
			std::string cLine, cName, cValue;
			size_t cFound;

			while ( ! ( getline( cFile, cLine ).eof() ) ){

				if( cLine.find_first_not_of( " \t" ) == std::string::npos ) continue;
				if( cLine.at(0) == '#' || cLine.at(0) =='*' ) continue;

				cFound = cLine.find( ":" );
				if( cFound == std::string::npos ) continue;

				cName = cLine.substr( 0, cFound-1 );
				cName.erase( remove_if( cName.begin(), cName.end(), isspace), cName.end() );

				cValue = cLine.substr( cFound + 1, std::string::npos );
				cValue.erase( remove_if( cValue.begin(), cValue.end(), isspace ), cValue.end() );

				// Here the Reg name sits in cName and the Reg value sits in cValue
				if( cValue.find("0x") != std::string::npos ){
					fRegMap[cName] = strtol( cValue.c_str(), 0, 16 );
				}
				else{
					fRegMap[cName] = strtol( cValue.c_str(), 0, 10 );
				}
			}

			cFile.close();

		}

	}

}
