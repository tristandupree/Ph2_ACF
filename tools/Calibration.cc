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
	void Calibration::Initialise()
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

		fBeBoardFWMap[0] = new GlibFWInterface(UHAL_CONNECTION_FILE,0);
		fBeBoardVec.push_back(new BeBoard(0,0));

		fBeBoardInterface.ConfigureBoard(fBeBoardVec[0]);

		fModule = new Module(fBeBoardVec[0]->fShelveId,fBeBoardVec[0]->fBeId,0,0,0);
		fBeBoardVec[0]->addModule(*fModule);
		delete fModule;

		for(uint8_t lcbc=0; lcbc<8; lcbc++)
		{
			fCbc = new Cbc(0,0,0,0,lcbc,(boost::format("settings/Cbc_default_electron.txt").str()));
			fBeBoardVec[0]->getModule(0)->addCbc(*fCbc);
			fCbcInterface.ConfigureCbc(fBeBoardVec[0]->getModule(0)->getCbc(lcbc));
			delete fCbc;

			std::cout<<"Cbc: "<<int(lcbc)<<"created!"<<std::endl;
			
			for(uint8_t lgroup=0; lgroup<8; lgroup++)
			{
				TestGroup fTestgroup(0,0,lcbc,lgroup);
				
				std::cout<<"	Group: "<<int(lgroup)<<"created!"<<std::endl;

				std::vector<Channel> cChannelVect;
				for(uint8_t lchannel=0; lchannel<16; lchannel++)
				{
					
					if (lchannel*16+lgroup*2 < 254)
					{
						Channel cChannel(0,0,lcbc, lchannel*16+lgroup*2 );
						std::cout<<"		Channel: "<<int(lchannel*16+lgroup*2)<<"created!"<<std::endl;
						cChannelVect.push_back(cChannel);
					}

					if ((lchannel*16+lgroup*2) +1 < 254)
					{
						Channel cChannel(0,0,lcbc, (lchannel*16+lgroup*2) +1 );
						std::cout<<"		Channel: "<<int( (lchannel*16+lgroup*2) +1)<<"created!"<<std::endl;
						cChannelVect.push_back(cChannel);
					}

				}

				fTestGroupMap[fTestgroup]=cChannelVect;
			}
		}
		
	}


	void Calibration::ConfigureBoard()
	{
		fBeBoardInterface.ConfigureBoard(fBeBoardVec[0]);
	}

	void Calibration::ConfigureCbc()
	{
		for(uint8_t i=0; i<8; i++)
		{
			fCbcInterface.ConfigureCbc(fBeBoardVec[0]->getModule(0)->getCbc(i));
		}
	}

}
