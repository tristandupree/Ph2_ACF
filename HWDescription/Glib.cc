#include "Glib.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <fstream>

namespace Ph2_HwDescription{

	// Constructors
	Glib::Glib():fShelveId( 0 ), fBeId( 0 ){

		loadConfigFile( default_glib_file );

	}

	Glib::Glib( uint8_t pShelveId, uint8_t pBeId, uint8_t pNFe, std::string filename ):fShelveId( pShelveId ),fBeId( pBeId ){

		fModuleVector.reserve(pNFe);
		loadConfigFile( filename );

	}

	Glib::Glib( uint8_t pShelveId, uint8_t pBeId, uint8_t pNFe, uint8_t pFMCConfiguration, bool pExtTrg, bool pFakeData ,  std::string filename):fShelveId( pShelveId ),fBeId( pBeId ), fFMCConfiguration(pFMCConfiguration), fExtTrg(pExtTrg)
	{
		fModuleVector.reserve(pNFe);
		loadConfigFile( filename );
	}

	// Public Members:


	uint8_t Glib::getReg( std::string pReg )
	{
		GlibRegMap::iterator i;
		i = fRegMap.find( pReg );
		if( i == fRegMap.end() ) 
		{
			std::cout << "Register " << pReg << " not found in Glib Register Map!" << std::endl;
			return 0;
		}
		else return i->second;
	}

	void Glib::setReg( std::string pReg, uint8_t psetValue )
	{
		GlibRegMap::iterator i;
		i = fRegMap.find( pReg );
		if( i == fRegMap.end() ) std::cout << "Register " << pReg << " not found in Glib Register Map!" << std::endl;
		else fRegMap[pReg] = psetValue;
	}

	void Glib::addModule( Module& pModule )
	{
		fModuleVector.push_back(pModule);
	}

	bool Glib::removeModule( uint8_t pModuleId )
	{
		std::vector < Module > :: iterator i;
		bool j=false;
		for (i=fModuleVector.begin();i!=fModuleVector.end();++i)
		{
			if (i->fFeId==pModuleId)
			{
				fModuleVector.erase(i);
				j=true;
				i--;   //erase reduces the container size by the number of elements removed, which are destroyed. To avoid that the iterator point an unallocated part of the memory, we need to decrease the iterator 
			}
		}
		if (j==true)
		return true;
		else
		{
			std::cout<<"Error:This Glib doesn't have the module "<<pModuleId<<std::endl;
			return false;
		}	
	}

	Module Glib::getModule( uint8_t pModuleId )
	{
		Module obj;
		std::vector < Module > :: iterator i;
		for (i=fModuleVector.begin();i!=fModuleVector.end();++i)
		{
			if (i->fFeId==pModuleId)
				{
					return *i;
				}
		}
		return obj;
	}

	// Private Members:

	void Glib::loadConfigFile( std::string filename )

	{

		std::ifstream cFile( filename.c_str(),std::ios::in );
		if( ! cFile ) std::cerr << "The Glib Settings File " << filename << " could not be opened!" << std::endl;
		else
		{

			std::cout << "Reading Glib settings file " << filename << std::endl;

			fRegMap.clear();
			std::string cLine, cName, cValue, cFound;

			while ( ! ( getline( cFile, cLine ).eof() ) )
			{

				if( cLine.find_first_not_of( " \t" ) == std::string::npos ) continue; 
				if( cLine.at(0) == '#' || cLine.at(0) =='*' ) continue;
				if( cLine.find( ":" ) == std::string::npos ) continue;

				std::istringstream input(cLine);
				input>> cName >> cFound>> cValue;


				// Here the Reg name sits in cName and the Reg value sits in cValue
				if( cValue.find("0x") != std::string::npos )
				{
					fRegMap[cName] = strtol( cValue.c_str(), 0, 16 );
				}
				else
				{
					fRegMap[cName] = strtol( cValue.c_str(), 0, 10 );
				}
			}

			cFile.close();

		}

	}

}
