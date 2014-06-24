#include "Cbc.h"
#include <fstream>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <string.h>
#include <iomanip>


namespace Ph2_HwDescription{


	// Default C'tor
	Cbc::Cbc():FEDescription()
	{
		fCbcId=0;
		loadfRegMap(default_file);

	 }



	// C'tors with object FE Description

	Cbc::Cbc( FEDescription& pFeDesc, uint8_t pCbcId, std::string filename ):FEDescription(pFeDesc)
	{

		fCbcId=pCbcId;
		loadfRegMap(filename);
	}


	Cbc::Cbc( FEDescription& pFeDesc, uint8_t pCbcId,uint8_t pTriggerLatency,uint8_t pVcth ):FEDescription(pFeDesc)
	{
		fCbcId=pCbcId;

		loadfRegMap(default_file);

		fRegMap["TriggerLatency"].fValue=pTriggerLatency;
		fRegMap["Vcth"].fValue=pVcth;

	}


	Cbc::Cbc( FEDescription& pFeDesc, uint8_t pCbcId ):FEDescription(pFeDesc)
	{
		fCbcId=pCbcId;
		loadfRegMap(default_file);
	}


	// C'tors which take ShelveID, BeId, FMCId, FeID, CbcId

	Cbc::Cbc( uint8_t pShelveId, uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId, uint8_t pCbcId, std::string filename ):FEDescription(pShelveId,pBeId,pFMCId,pFeId)
	{

		fCbcId=pCbcId;
		loadfRegMap(filename);
	}


	Cbc::Cbc( uint8_t pShelveId, uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId, uint8_t pCbcId, uint8_t pTriggerLatency,uint8_t pVcth ):FEDescription(pShelveId,pBeId,pFMCId,pFeId)
	{
		fCbcId=pCbcId;

		loadfRegMap(default_file);

		fRegMap["TriggerLatency"].fValue=pTriggerLatency;
		fRegMap["Vcth"].fValue=pVcth;
	}


	Cbc::Cbc( uint8_t pShelveId, uint8_t pBeId, uint8_t pFMCId, uint8_t pFeId, uint8_t pCbcId ):FEDescription(pShelveId,pBeId,pFMCId,pFeId)
	{
		fCbcId=pCbcId;
		loadfRegMap(default_file);

	}


	// Copy C'tor

	Cbc::Cbc(const Cbc& cbcobj):FEDescription(cbcobj)
	{
		fCbcId=cbcobj.fCbcId;
		fRegMap=cbcobj.fRegMap;
	}


	// D'Tor

	Cbc::~Cbc()
	{

	}


	//load fRegMap from file

	void Cbc::loadfRegMap(std::string filename)
	{
		std::ifstream file(filename.c_str(),std::ios::in);

		if (file)
		{
			std::string line,fName, fPage_str, fAddress_str, fDefValue_str, fValue_str;
			CbcRegItem fRegItem;

			while (getline(file,line))
			{
				if( line.find_first_not_of( " \t" ) == std::string::npos ) continue;
				if( line.at(0) == '#' || line.at(0) =='*' ) continue;
				std::istringstream input(line);
				input>> fName >> fPage_str >> fAddress_str >> fDefValue_str >> fValue_str;

				fRegItem.fPage=strtoul( fPage_str.c_str(), 0, 16 );
				fRegItem.fAddress=strtoul( fAddress_str.c_str(), 0, 16 );
				fRegItem.fDefValue=strtoul( fDefValue_str.c_str(), 0, 16 );
				fRegItem.fValue=strtoul( fValue_str.c_str(), 0, 16 );

				fRegMap[fName]=fRegItem;
			}

			file.close();
		}
		else
			std::cerr<< "The CBC Settins File " << filename << " could not be opened!" <<std::endl;
	}

	uint8_t Cbc::getTriggerLatency()
	{
		CbcRegMap::iterator i;
		i=fRegMap.find("TriggerLatency");
		if (i==fRegMap.end())
		{std::cout<<"This Cbc object doesn't have TriggerLatency"<<std::endl;
		return 0;}
		else
		return i->second.fValue;
	}

	void Cbc::setTriggerLatency(uint8_t pTriggerLatency)
	{
		CbcRegMap::iterator i;
		i=fRegMap.find("TriggerLatency");
		if (i==fRegMap.end())
		{std::cout<<"This Cbc object doesn't have TriggerLatency"<<std::endl;}
		else
		{
			i->second.fValue=pTriggerLatency;
		}
	}

	uint8_t Cbc::getVcth()
	{
		CbcRegMap::iterator i;
		i=fRegMap.find("VCth");
		if (i==fRegMap.end())
		{std::cout<<"This Cbc object doesn't have Vcth"<<std::endl;
		return 0;}
		else
		return i->second.fValue;
	}

	void Cbc::setVcth(uint8_t psetVcth)
	{
		CbcRegMap::iterator i;
		i=fRegMap.find("VCth");
		if (i==fRegMap.end())
		{std::cout<<"This Cbc object doesn't have Vcth"<<std::endl;}
		else
		{
			i->second.fValue=psetVcth;

		}
	}


	uint8_t Cbc::getReg(std::string pReg)
	{
		CbcRegMap::iterator i;
		i=fRegMap.find(pReg);
		if (i==fRegMap.end())
		{
			std::cout<<"This Cbc object doesn't have "<<pReg.c_str()<<std::endl;
			return 0;
		}
		else
		return i->second.fValue;
	}


	void Cbc::setReg(std::string pReg, uint8_t psetValue)
	{
		CbcRegMap::iterator i;
		i=fRegMap.find(pReg);
		if (i==fRegMap.end())
		{std::cout<<"This Cbc object doesn't have "<<pReg.c_str()<<std::endl;}
		else
		{
			i->second.fValue=psetValue;

		}
	}


	//Write RegValues in a file

	void Cbc::writeRegValues( std::string filename )
	{

		std::ofstream file(filename.c_str(), std::ios::out | std::ios::trunc);

        	if(file)
       		{
			file<< "* RegName";
			for (int j=0;j<48;j++) {file<<" ";}
			file.seekp(-strlen("* RegName"),std::ios_base::cur);

			file<< "Page\tAddr\tDefval\tValue" << std::endl;
		file<<"*--------------------------------------------------------------------------------"<<std::endl;

			CbcRegMap::iterator i;
			for (i=fRegMap.begin();i!=fRegMap.end();++i)
			{

			file<< i->first;
			for (int j=0;j<48;j++) {file<<" ";}
			file.seekp(-i->first.size(),std::ios_base::cur);

			std::string fpage_str;


                	file<<"0x" << std::setfill ('0') << std::setw (2) << std::hex<< uint32_t(i->second.fPage) <<"\t0x" << std::setfill ('0') << std::setw (2)<< std::hex<< uint32_t(i->second.fAddress) <<"\t0x" << std::setfill ('0') << std::setw (2)<< std::hex<< uint32_t(i->second.fDefValue) <<"\t0x" << std::setfill ('0') << std::setw (2)<< std::hex<< uint32_t(i->second.fValue) <<std::endl;

       			}
			file.close();
       		}
		else
                std::cerr << "Error opening file" << std::endl;
	}


	bool CbcComparer::operator() (const Cbc& cbc1,const Cbc& cbc2)
	{
		if (cbc1.fShelveId != cbc2.fShelveId) return cbc1.fShelveId < cbc2.fShelveId;
		else if(cbc1.fBeId != cbc2.fBeId) return cbc1.fBeId < cbc2.fBeId;
		else if(cbc1.fFMCId != cbc2.fFMCId) return cbc1.fFMCId < cbc2.fFMCId;
		else if(cbc1.fFeId != cbc2.fFeId) return cbc1.fFeId < cbc2.fFeId;
		else return cbc1.fCbcId < cbc2.fCbcId ;
	}

}
