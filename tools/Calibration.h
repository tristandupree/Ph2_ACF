/*!
*
* \file Calibration.h
* \brief Calibration class, calibration of the hardware
* \author Lorenzo BIDEGAIN
* \date 13/08/14
*
* Support : mail to : lorenzo.bidegain@cern.ch
*
*/

#ifndef Calibration_h__
#define Calibration_h__

#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "Channel.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;


/*!
* \namespace Ph2_HwTools
* \brief Namespace regrouping all the hardware tools
*/
namespace Ph2_HwTools{

	/*!
	* \class Calibration
	* \brief Read/Write Cbc's registers on a file
	*/
	class Calibration {

	public:
			
		// Default C'tor
		Calibration():
		fBeBoardInterface(fBeBoardFWMap),
		fCbcInterface(fBeBoardFWMap)
		{};

		// D'Tor
		~Calibration(){};

		/*!
		* \brief Initialise calibration
		*/
		void Initialise();
		/*!
		* \brief Configure the Board
		*/
		void ConfigureBoard();
		/*!
		* \brief Configure the Cbc's
		*/
		void ConfigureCbc();

	private:
		
		std::map < std::string, uint8_t > fSettingsMap;

		TestGroupMap fTestGroupMap;

		//Representation of the Hardware (hardcoding for the moment)
		BeBoard* fBeBoard;
    		Module* fModule;
    		Cbc* fCbc;

		BeBoardFWMap fBeBoardFWMap;
    		std::vector<BeBoard*> fBeBoardVec;

    		BeBoardInterface fBeBoardInterface;
    		CbcInterface fCbcInterface;


	};

}

#endif
