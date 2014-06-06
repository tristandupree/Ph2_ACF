/*

  FileName : 					HWInterface.cpp
  Content : 					 HWInterface class, permit connection & r/w registers
  Programmer : 				Nicolas PIERRE
  Version : 					 1.0
  Date of creation : 	06/06/14
  Support : 					 mail to : nicolas.pierre@etu.unistra.net

*/
#include <uhal/uhal.hpp>
#include "HWInterface.h"

namespace Interface
{

  HWInterface::HWInterface(const char *puHalConfigFileName, const char *pBoardId)
  {
    uhal::ConnectionManager cm( puHalConfigFileName ); // Get connection

    fBoard = new uhal::HwInterface( cm.getDevice( pBoardId ) ); // New HwInterface
  }


  HWInterface::~HWInterface()
  {
    delete fBoard;
  }


  // Give the node of the reg and the value you want to write
  bool HWInterface::WriteReg(const std::string& pRegNode, const uint32_t& pVal)
  {
    fBoard->getNode(pRegNode).write(pVal);
    fBoard->dispatch();

    // Verify if the writting is done correctly
    uhal::ValWord<uint32_t> reply = fBoard->getNode(pRegNode).read();
		fBoard->dispatch();

    uint32_t comp = (uint32_t) reply ;

    if(comp==pVal)
      return true;

    std::cout << "\nValues are not consistent : \nExpected : " << pVal << "\nActual : " << comp << std::endl;

    return false;
  }


  // Give the node of the reg you want to read
  void HWInterface::ReadReg(const std::string& pRegNode)
  {
    uhal::ValWord<uint32_t> ValRead = fBoard->getNode(pRegNode).read();
    fBoard->dispatch();

    uint32_t read = (uint32_t) ValRead ;

    std::cout << "\nValue in register ID " << pRegNode << " : " << read << std::endl;
  }
}
