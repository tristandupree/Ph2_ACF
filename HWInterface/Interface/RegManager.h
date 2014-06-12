/*

	FileName : 					RegManager.h
	Content : 					 RegManager class, permit connection & r/w registers
	Programmer : 				  Nicolas PIERRE
	Version : 					 1.2
	Date of creation : 	        06/06/14
	Support : 					 mail to : nicolas.pierre@etu.unistra.net

*/
#ifndef __REGMANAGER_H__
#define __REGMANAGER_H__

#include <string>
#include <map>
#include <vector>
#include <uhal/uhal.hpp>

namespace Ph2_HwInterface
{
  class RegManager
  {
    protected:
      uhal::HwInterface *fBoard;


    public:
      RegManager(const char *puHalConfigFileName, const char *pBoardId); // Connection w uHal
      virtual ~RegManager();

      virtual bool WriteReg(const std::string& pRegNode, const uint32_t& pVal);
      virtual uhal::ValWord<uint32_t> ReadReg(const std::string& pRegNode);
      virtual uhal::ValVector<uint32_t> ReadBlockReg(const std::string& pRegNode, const uint32_t& pBlocksize);
  };
}

#endif
