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

	InitializeHw("/afs/cern.ch/user/l/lbidegai/public/new_archi/System/HWDescription.xml");



	uint32_t cMissedBoard, cMissedModule, cMissedCbc;

	for(uint32_t lShelve=0; lShelve<fShelveVec.size(); lShelve++)
	{
	  cMissedBoard = 0;

	  for(uint32_t lBoard=0; lBoard<fShelveVec[lShelve]->getNBoard(); lBoard++)
	  {
	    if(fShelveVec[lShelve]->getBoard(lBoard+cMissedBoard) == NULL)
	    {
	      lBoard--;
	      cMissedBoard++;
	    }

	    else
	    {
	      cMissedModule = 0;

              for(uint32_t lFe=0; lFe<fShelveVec[lShelve]->getBoard(lBoard+cMissedBoard)->getNFe(); lFe++)
	      {

		if(fShelveVec[lShelve]->getBoard(lBoard+cMissedBoard)->getModule(lFe+cMissedModule) == NULL)
		{
		  lFe--;
		  cMissedModule++;
		}

		else
		{
		  cMissedCbc = 0;

		  for(uint32_t lcbc=0; lcbc<fShelveVec[lShelve]->getBoard(lBoard+cMissedBoard)->getModule(lFe+cMissedModule)->getNCbc(); lcbc++)
		  {
			if(fShelveVec[lShelve]->getBoard(lBoard+cMissedBoard)->getModule(lBoard+cMissedModule)->getCbc(lcbc+cMissedCbc) == NULL)
		  {
			lcbc--;
		 	cMissedCbc++;
		  }

		  else
		  {

		  	std::cout<<"Cbc: "<<int(lcbc)<<"created!"<<std::endl;
			ConstructTestGroup(lBoard, lFe, lcbc);	

		  }

		  }
	        }
	      }
	    }
	  }
	}	
}


void Calibration::ConstructTestGroup(uint8_t pBeId, uint8_t FeId, uint8_t CbcId)
{
	for(uint8_t lgroup=0; lgroup<8; lgroup++)
	{
		TestGroup fTestgroup(pBeId,FeId,CbcId,lgroup);
				
		std::cout<<"	Group: "<<int(lgroup)<<"created!"<<std::endl;

		std::vector<Channel> cChannelVect;
		for(uint8_t lchannel=0; lchannel<16; lchannel++)
		{
					
			if (lchannel*16+lgroup*2 < 254)
			{
				Channel cChannel(pBeId,FeId,CbcId, lchannel*16+lgroup*2 );
				std::cout<<"		Channel: "<<int(lchannel*16+lgroup*2)<<"created!"<<std::endl;
				cChannelVect.push_back(cChannel);
			}

			if ((lchannel*16+lgroup*2) +1 < 254)
			{
				Channel cChannel(pBeId,FeId,CbcId, (lchannel*16+lgroup*2) +1 );
				std::cout<<"		Channel: "<<int( (lchannel*16+lgroup*2) +1)<<"created!"<<std::endl;
				cChannelVect.push_back(cChannel);
			}

		}

		fTestGroupMap[fTestgroup]=cChannelVect;
	}

}	
	



