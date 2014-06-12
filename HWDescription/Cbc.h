#ifndef Cbc_h__
#define Cbc_h__

#include "FEDescription.h"
#include "CbcRegItem.h"
#include <iostream>
#include <map>
#include <string>

#include "RTypes.h"

// Cbc2 Chip HW Description Class

namespace Ph2_HwDescription{

	//class CbcRegItem;

	typedef std::map < std::string, CbcRegItem > CbcRegMap; 

	class Cbc : public FEDescription{

	public:

		// C'tors with object FE Description 
		Cbc( FEDescription& pFeDesc, UInt_t pCbcId, std::string filename );
		Cbc( FEDescription& pFeDesc, UInt_t pCbcId,UInt_t pTriggerLatency,UInt_t pVcth );
		Cbc( FEDescription& pFeDesc, UInt_t pCbcId ); 

		// C'tors which take ShelveID, BeId, FeID, CbcId
		Cbc( UInt_t pShelveId, UInt_t pBeId, UInt_t pFMCId, UInt_t pFeId, UInt_t pCbcId, std::string filename );
		Cbc( UInt_t pShelveId, UInt_t pBeId, UInt_t pFMCId, UInt_t pFeId, UInt_t pCbcId, UInt_t pTriggerLatency,UInt_t pVcth );
		Cbc( UInt_t pShelveId, UInt_t pBeId, UInt_t pFMCId, UInt_t pFeId, UInt_t pCbcId );

		// Default C'tor
		Cbc();

		// Copy C'tor
		Cbc(Cbc& cbcobj);

		//load fRegMap from file
		void loadfRegMap(std::string filename);		


		UInt_t getTriggerLatency();
		void setTriggerLatency(UInt_t pTriggerLatency);

		UInt_t getVcth();
		void setVcth(UInt_t psetVcth);

		void dumpRegValues( std::string filename );
		void updateRegValues( std::string filename );

		//functor
		bool operator()(Cbc& cbcobj)
		{return (fRegMap["VCth"].fValue<cbcobj.fRegMap["VCth"].fValue);};

	protected:

		// Map of Register Name vs. RegisterItem that contains: Page, Address, Default Value, Value
		CbcRegMap fRegMap;
		UInt_t fCbcId;

	private:

		// CBC Register values, all in hex representation
		// think of a better (more lightweight datatype for hex representation of register values: 8 bit each, range from 0 to 255)
		// Lorenzo: Do we have to keep this attributes? the values of these register will be already charge in the memory thanks to the std::map
		/*UInt_t fFrontEndControl;                              	
		UInt_t fTriggerLatency;                               	
		UInt_t fHitDetectSLVS ;                               	
		UInt_t fIpre1;                                       	
		UInt_t fIpre2;                                        	
		UInt_t fIpsf;                                         	
		UInt_t fIpa;                                          	
		UInt_t fIpaos;                                        	
		UInt_t fVpafb;                                        	
		UInt_t fIcomp;                                        	
		UInt_t fVpc;                                          	
		UInt_t fVplus;                                        	
		UInt_t fVCth;                                         	
		UInt_t fTestPulsePot;   
		// Reg Name is SelTestPulseDel&ChanGroup                   	
		UInt_t fSelTestPulseDelChanGroup;                    	
		// Reg Name is MiscTestPulseCtrl&AnalogMux 
		UInt_t fMiscTestPulseCtrlAnalogMux;                  	
		UInt_t fTestPulseChargePumpCurrent;                   	
		UInt_t fTestPulseChargeMirrCascodeVolt;               	
		// Reg Name is CwdWindow&Coincid
		UInt_t fCwdWindowCoincid;                            	
		UInt_t fMiscStubLogic;
		// Mask channels in Groups of 8
		bool   fMaskChanGroup[33];
		// Channel Offsets
		UInt_t fChannelOffset[254];  */  

	};
}


#endif
