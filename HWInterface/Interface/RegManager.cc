/*

  FileName : 					RegManager.cc
  Content : 					 RegManager class, permit connection & r/w registers
  Programmer : 				  Nicolas PIERRE
  Version : 					 1.2
  Date of creation : 	        06/06/14
  Support : 					 mail to : nicolas.pierre@etu.unistra.net

*/
#include <uhal/uhal.hpp>
#include "RegManager.h"

#define DEV_FLAG    0

namespace Ph2_HwInterface
{

  RegManager::RegManager(const char *puHalConfigFileName, const char *pBoardId)
  {
    uhal::ConnectionManager cm( puHalConfigFileName ); // Get connection

    fBoard = new uhal::HwInterface( cm.getDevice( pBoardId ) ); // New HwInterface
  }


  RegManager::~RegManager()
  {
    delete fBoard;
  }


  // Give the node of the reg and the value you want to write
  bool RegManager::WriteReg(const std::string& pRegNode, const uint32_t& pVal)
  {
    fBoard->getNode(pRegNode).write(pVal);
    fBoard->dispatch();

    // Verify if the writing is done correctly
    if (DEV_FLAG)
    {
        uhal::ValWord<uint32_t> reply = fBoard->getNode(pRegNode).read();
        fBoard->dispatch();

        uint32_t comp = (uint32_t) reply ;

        if(comp==pVal)
            return true;

        std::cout << "\nERROR !!\nValues are not consistent : \nExpected : " << pVal << "\nActual : " << comp << std::endl;
    }

    return false;
  }


  // Give the node of the reg you want to read
  uhal::ValWord<uint32_t> RegManager::ReadReg(const std::string& pRegNode)
  {
    uhal::ValWord<uint32_t> cValRead = fBoard->getNode(pRegNode).read();
    fBoard->dispatch();

    if (DEV_FLAG)
    {
        uint32_t read = (uint32_t) cValRead ;
        std::cout << "\nValue in register ID " << pRegNode << " : " << read << std::endl;
    }

    return cValRead;
  }

  uhal::ValVector<uint32_t> RegManager::ReadBlockReg(const std::string& pRegNode, const uint32_t& pBlockSize)
  {
    //uint32_t read;

    uhal::ValVector<uint32_t> cBlockRead = fBoard->getNode(pRegNode).readBlock(pBlockSize);
    fBoard->dispatch();

    /*
    Not working as for now (iterator issue)

    if (DEV_FLAG)
    {
        std::cout << "\nValues in register block " << pRegNode << " : " << std::endl;

        for(uhal::ValVector<uint32_t>::iterator cIt = cBlockRead.begin(); cIt != cBlockRead.end(); cIt++ )
        {
            read = (uint32_t) cBlockRead[cIt];
            std::cout << " " << read << " " << std::endl;
        }
    }
    */
    return cBlockRead;

  }
}
