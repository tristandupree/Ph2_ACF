/*

	FileName : 					HWInterface.h
	Content : 					 HWInterface class, permit connection & r/w registers
	Programmer : 				Nicolas PIERRE
	Version : 					 1.0
	Date of creation : 	06/06/14
	Support : 					 mail to : nicolas.pierre@etu.unistra.net

*/
#ifndef __HWINTERFACE_H__
#define __HWINTERFACE_H__

#include <string>
#include <map>
#include <vector>
#include <uhal/uhal.hpp>

namespace Interface
{
  class HWInterface
  {
    private:
      uhal::HwInterface *fBoard;


    public:
      HWInterface(const char *puHalConfigFileName, const char *pBoardId); // Connection w uHal
      ~HWInterface();

      bool WriteReg(const std::string& pRegNode, const uint32_t& pVal);
      void ReadReg(const std::string& pRegNode);
  };
}

#endif
