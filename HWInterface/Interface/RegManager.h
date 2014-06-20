/*

    FileName :                    RegManager.h
    Content :                     RegManager class, permit connection & r/w registers
    Programmer :                  Nicolas PIERRE
    Version :                     1.3
    Date of creation :            06/06/14
    Support :                     mail to : nicolas.pierre@cern.ch

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
            const char *fUHalConfigFileName;
            std::map<uint8_t,uhal::HwInterface*> fBoardMap;

        public:
            /// Connection w uHal
            RegManager(const char *puHalConfigFileName);
            virtual ~RegManager();

            virtual bool WriteReg(const std::string& pRegNode, const uint32_t& pVal);
            virtual bool WriteBlockReg(const std::string& pRegNode, const std::vector< uint32_t >& pValues);
            virtual uhal::ValWord<uint32_t> ReadReg(const std::string& pRegNode);
            virtual uhal::ValVector<uint32_t> ReadBlockReg(const std::string& pRegNode, const uint32_t& pBlocksize);
            /// Choose the board we want to talk with
            virtual void ChooseBoard(uint8_t pBoardId);

    };
}

#endif
