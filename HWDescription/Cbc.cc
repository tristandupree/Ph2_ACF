#include "Cbc.h"
#include <fstream>
#include <cstdio>
#include <sstream>
#include <iostream>


namespace HWDescription{


	// Default C'tor
	Cbc::Cbc():FEDescription()
	{	
		fCbcId=0;
		loadfRegMap("default_file.txt");
		
	 }
	
			 

	// C'tors with object FE Description 

	Cbc::Cbc( FEDescription& pFeDesc, int pCbcId, std::string filename ):FEDescription(pFeDesc)
	{

		fCbcId=pCbcId;
		loadfRegMap(filename);
	}


	Cbc::Cbc( FEDescription& pFeDesc, int pCbcId,int pTriggerLatency,int pVcth ):FEDescription(pFeDesc)
	{
		fCbcId=pCbcId;

		loadfRegMap("default_file.txt");

		fRegMap["TriggerLatency"].fValue=pTriggerLatency;
		fRegMap["Vcth"].fValue=pVcth;
		
	}	


	Cbc::Cbc( FEDescription& pFeDesc, int pCbcId ):FEDescription(pFeDesc)
	{
		fCbcId=pCbcId;
		loadfRegMap("default_file.txt");
	}

	
	// C'tors which take ShelveID, BeId, FMCId, FeID, CbcId

	Cbc::Cbc( int pShelveId, int pBeId, int pFMCId, int pFeId, int pCbcId, std::string filename ):FEDescription(pShelveId,pBeId,pFMCId,pFeId)  
	{
	
		fCbcId=pCbcId;
		
		std::ifstream file(filename.c_str(),std::ios::in);
		loadfRegMap(filename);
	}
		

	Cbc::Cbc( int pShelveId, int pBeId, int pFMCId, int pFeId, int pCbcId, int pTriggerLatency,int pVcth ):FEDescription(pShelveId,pBeId,pFMCId,pFeId)
	{
		fCbcId=pCbcId;	
	
		loadfRegMap("default_file.txt");

		fRegMap["TriggerLatency"].fValue=pTriggerLatency;
		fRegMap["Vcth"].fValue=pVcth;
	}


	Cbc::Cbc( int pShelveId, int pBeId, int pFMCId, int pFeId, int pCbcId ):FEDescription(pShelveId,pBeId,pFMCId,pFeId)
	{
		fCbcId=pCbcId;
		loadfRegMap("default_file.txt");

	}


	Cbc::Cbc(Cbc& cbcobj):FEDescription(cbcobj)
	{
		fCbcId=cbcobj.fCbcId;
		fRegMap=cbcobj.fRegMap;
	}


	void Cbc::loadfRegMap(std::string filename)
	{
		std::ifstream file("default_file.txt",std::ios::in);

		if (file)
		{
			std::string line,fName;
			CbcRegItem fRegItem;
			
			while (getline(file,line))
			{
				std::istringstream input(line);
				input>> fName >> fRegItem.fPage >> fRegItem.fAddress >> fRegItem.fDefValue >> fRegItem.fValue;

				fRegMap[fName]=fRegItem;
			}
			
			file.close();
		}
		else
			std::cerr<< "File error" <<std::endl;
	}

	int Cbc::getTriggerLatency()
	{
		CbcRegMap::iterator i;
		i=fRegMap.find("TriggerLatency");
		if (i==fRegMap.end())
		{std::cout<<"This Cbc object doesn't have TriggerLatency"<<std::endl;
		return 0;}
		else		
		return fRegMap["TriggerLatency"].fValue;
	}

	void Cbc::setTriggerLatency(int pTriggerLatency)
	{
		CbcRegMap::iterator i;
		i=fRegMap.find("TriggerLatency");
		if (i==fRegMap.end())
		{std::cout<<"This Cbc object doesn't have TriggerLatency"<<std::endl;}
		else
		{
			fRegMap["TriggerLatency"].fValue=pTriggerLatency;
		}
	}

	int Cbc::getVcth()
	{
		CbcRegMap::iterator i;
		i=fRegMap.find("VCth");
		if (i==fRegMap.end())
		{std::cout<<"This Cbc object doesn't have Vcth"<<std::endl;
		return 0;}
		else		
		return fRegMap["VCth"].fValue;
	}

	void Cbc::setVcth(int psetVcth)
	{
		CbcRegMap::iterator i;
		i=fRegMap.find("VCth");
		if (i==fRegMap.end())
		{std::cout<<"This Cbc object doesn't have Vcth"<<std::endl;}
		else
		{
			fRegMap["VCth"].fValue=psetVcth;

		}
	}

	void Cbc::dumpRegValues( std::string filename )
	{
		
		if( remove( filename.c_str() ) != 0 )
    		std::cerr<<"Error deleting file"<<std::endl;
 		else
    		std::cout<<"File successfully deleted"<<std::endl;
	}

	void Cbc::updateRegValues( std::string filename )
	{
		
		std::ofstream file(filename.c_str(), std::ios::out | std::ios::trunc); 
 
        	if(file)
       		{
			CbcRegMap::iterator i;
			for (i=fRegMap.begin();i!=fRegMap.end();++i)
			{
				
                	file<< i->first <<" "<< i->second.fPage <<" " << i->second.fAddress <<" " << i->second.fDefValue <<" " <<  i->second.fValue <<std::endl;
                	
       			}
			file.close();
       		}
		else
                std::cerr << "Error opening file" << std::endl;
	}
}
