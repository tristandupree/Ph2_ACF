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
#include "Channel.h"


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

		fBeBoardFWMap[0] = new GlibFWInterface(UHAL_CONNECTION_FILE,0)
		fBeBoardVec.push_back(new BeBoard(0,0));

		fBeBoardInterface.ConfigureBoard(fBeBoardVec[0]);

		fModule = new Module(fBeBoardVec[0]->fShelveId,fBeBoardVec[0]->fBeId,0,0,0);
		fBeBoardVec[0]->addModule(*fModule);
		delete fModule;

		for(uint8_t i=0; i<8; i++)
		{
			fCbc = new Cbc(0,0,0,0,i,(boost::format("settings/cbc_default.txt") %(uint32_t(i))).str());
			fBeBoardVec[0]->getModule(0)->addCbc(*fCbc);
			fCbcInterface.ConfigureCbc(fBeBoardVec[0]->getModule(0)->getCbc(i));
			delete fCbc;

			std::cout<<"Cbc: "i<<"created!"<<std::endl;
			
			for(uint8_t j=0; j<8; j++)
			{
				TestGroup fTestgroup(0,0,i,j);
				
				std::cout<<"	Group: "j<<"created!"<<std::endl;

				std::vector<Channel> cChannelVect;
				for(uint8_t k=0; k<16; k++)
				{
					Channel cChannel(0,0,i, k*16+j*2 );
					std::cout<<"		Channel: "k*16+j*2<<"created!"<<std::endl;
					cChannelVect.push_back=cChannel;
				}

				fTestGroupMap[fTestgroup]=cChannelVect;
			}
		}
		
	}


	void Calibration::ConfigureBoard()
	{
		fBeBoardInterface.ConfigureBoard(fBeBoardVec[0]);
	}

	void Calibration::ConfigureCbc();
	{
		for(uint8_t i=0; i<8; i++)
		{
			fCbcInterface.ConfigureCbc(fBeBoardVec[0]->getModule(0)->getCbc(i));
		}
	}

}
