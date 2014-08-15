#ifndef __SYSTEMCONTROLLER_H__
#define __SYSTEMCONTROLLER_H__

#include "../HWInterface/BeBoardFWInterface.h"
#include "../HWDescription/Shelve.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "pugixml.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <stdlib.h>

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

namespace Ph2_System
{

	typedef std::vector<Shelve*> ShelveVec;

	class SystemController
	{
		public:
			SystemController();
			~SystemController();
			// Public methods
			void InitializeHw(std::string pFilename);
			void ConfigureHw();
			void Run(BeBoard* pBeBoard, uint32_t pNthAcq);

			// Public members: user interfaces
			BeBoardInterface*	fBeBoardInterface;
			CbcInterface*		fCbcInterface;

			// BeBoardObjects contains all HW Description
			ShelveVec		 	fShelveVec;

			// BeBoardFWInterface Map
			BeBoardFWMap   		fBoardFWMap;
	};
}

#endif
