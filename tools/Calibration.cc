/*!
*
* \file Calibration.cc
* \brief Calibration class, calibration of the hardware
* \author Lorenzo BIDEGAIN
* \date 13/08/14
*
* Support : mail to : lorenzo.bidegain@cern.ch
*
*/

#include "Calibration.h"


namespace Ph2_HwTools{


	//No arguments for now, the hardware is hardcoding, it will be different with the xml file
	Calibration::Initialise()
	{

		//Initialise the Map
		fSettingsMap["1)RunNumber"]=1;
		fSettingsMap["2)BeamIntensity"]=0;
		fSettingsMap["3)Nevents"]=0;
		fSettingsMap["4)SaveData"]=0;
		fSettingsMap["EnableTestPulse"]=0;
		fSettingsMap["TestGroup"]=0;
		fSettingsMap["TestPulsePotentiometer"]=0xF1;

		//Representation of the Hardware (hardcoding for the moment)
		BeBoard* cBeBoard;
    		Module* cModule;
    		Cbc* cCbc;

		BeBoardFWMap cBeBoardFWMap;
    		std::vector<BeBoard*> cBeBoardVec;

    		BeBoardInterface cBeBoardInterface(cBeBoardFWMap);
    		CbcInterface cCbcInterface(cBeBoardFWMap);

		cBeBoardFWMap[0] = new GlibFWInterface(UHAL_CONNECTION_FILE,cBoardId)
		cBeBoardVec.push_back(new BeBoard(0,0));

		cBeBoardInterface.ConfigureBoard(cBeBoardVec[0]);

		cModule = new Module(cBeBoardVec[0]->fShelveId,cBeBoardVec[0]->fBeId,0,0,0);
		cBeBoardVec[0]->addModule(*cModule);
		delete cModule;

		for(uint8_t i=0; i<8; i++)
		{
			cCbc = new Cbc(0,0,0,0,i,(boost::format("settings/cbc_default.txt") %(uint32_t(i))).str());
			cBeBoardVec[0]->getModule(0)->addCbc(*cCbc);
			cCbcInterface.ConfigureCbc(cBeBoardVec[0]->getModule(0)->getCbc(i));
			delete cCbc;
		}
		
	}


	void Calibration::ConfigureBoard()
	{
		cBeBoardInterface.ConfigureBoard(cBeBoardVec[0]);
	}

	void Calibration::ConfigureCbc();
	{
		for(uint8_t i=0; i<8; i++)
		{
			cCbcInterface.ConfigureCbc(cBeBoardVec[0]->getModule(0)->getCbc(i));
		}
	}

}
