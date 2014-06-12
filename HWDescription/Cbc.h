#ifndef Cbc_h__
#define Cbc_h__

#include "FEDescription.h"
#include "CbcRegItem.h"
#include <iostream>
#include <map>
#include <string>

// Cbc2 Chip HW Description Class

namespace HWDescription{

	//class CbcRegItem;

	typedef std::map < std::string, CbcRegItem > CbcRegMap; 

	class Cbc : public FEDescription{

	public:

		// C'tors with object FE Description 
		Cbc( FEDescription& pFeDesc, int pCbcId, std::string filename );
		Cbc( FEDescription& pFeDesc, int pCbcId,int pTriggerLatency,int pVcth );
		Cbc( FEDescription& pFeDesc, int pCbcId ); 

		// C'tors which take ShelveID, BeId, FeID, CbcId
		Cbc( int pShelveId, int pBeId, int pFMCId, int pFeId, int pCbcId, std::string filename );
		Cbc( int pShelveId, int pBeId, int pFMCId, int pFeId, int pCbcId, int pTriggerLatency,int pVcth );
		Cbc( int pShelveId, int pBeId, int pFMCId, int pFeId, int pCbcId );

		// Default C'tor
		Cbc();

		// Copy C'tor
		Cbc(Cbc& cbcobj);

		//load fRegMap from file
		void loadfRegMap(std::string filename);		


		int getTriggerLatency();
		void setTriggerLatency(int pTriggerLatency);

		int getVcth();
		void setVcth(int psetVcth);

		void dumpRegValues( std::string filename );
		void updateRegValues( std::string filename );

		//functor
		bool operator()(Cbc& cbcobj)
		{return (fRegMap["VCth"].fValue<cbcobj.fRegMap["VCth"].fValue);};

	protected:

		// Map of Register Name vs. RegisterItem that contains: Page, Address, Default Value, Value
		CbcRegMap fRegMap;
		int fCbcId;

	private:

		// CBC Register values, all in hex representation
		// think of a better (more lightweight datatype for hex representation of register values: 8 bit each, range from 0 to 255)
		// Lorenzo: Do we have to keep this attributes? the values of these register will be already charge in the memory thanks to the std::map
		/*int fFrontEndControl;                              	
		int fTriggerLatency;                               	
		int fHitDetectSLVS ;                               	
		int fIpre1;                                       	
		int fIpre2;                                        	
		int fIpsf;                                         	
		int fIpa;                                          	
		int fIpaos;                                        	
		int fVpafb;                                        	
		int fIcomp;                                        	
		int fVpc;                                          	
		int fVplus;                                        	
		int fVCth;                                         	
		int fTestPulsePot;   
		// Reg Name is SelTestPulseDel&ChanGroup                   	
		int fSelTestPulseDelChanGroup;                    	
		// Reg Name is MiscTestPulseCtrl&AnalogMux 
		int fMiscTestPulseCtrlAnalogMux;                  	
		int fTestPulseChargePumpCurrent;                   	
		int fTestPulseChargeMirrCascodeVolt;               	
		// Reg Name is CwdWindow&Coincid
		int fCwdWindowCoincid;                            	
		int fMiscStubLogic;
		// Mask channels in Groups of 8
		bool   fMaskChanGroup[33];
		// Channel Offsets
		int fChannelOffset[254];  */  

	};
}


#endif
