/*

  FileName : 					GLIBInterface.h
  Content : 					 GLIBInterface class, init/config of the Glib
  Programmer : 				  Nicolas PIERRE
  Version : 					 0.0
  Date of creation : 	        07/06/14
  Support : 					 mail to : nicolas.pierre@etu.unistra.net

*/
#ifndef __GLIBINTERFACE_H__
#define __GLIBINTERFACE_H__

#include <string>
#include <map>
#include <vector>
#include <uhal/uhal.hpp>

namespace Ph2_HwInterface
{
    class GLIBInterface: public RegManager
    {
        private:
			std::string                     fUhalConfigFileName;
			std::string                     fGLIBId;
			std::string                     fOutputDir;
            //map<string,(u)int>              fGlibSettings;

			unsigned int				    fBeId;
			unsigned int                    fNFe;
			unsigned int                    fNCbc;
            unsigned int                    fNeventPerAcq;
            unsigned int                    fNTotalAcq;
			bool                            fNegativeLogicCBC;
			bool                            fStop;

            struct timeval fStartVeto;
            std::string fStrSram, fStrSramUserLogic, fStrFull, fStrReadout;

            uhal::ValVector<uint32_t> fData;

        public:
            static unsigned int NBe;
            static unsigned int fPacketSize;

        public:
            //Constructor, takes a GLIB object as parameter
            GLIBInterface(const char *pConfigFile, int pCbcNb/*, GLIB& pGLIB*/);
            //Destructor
            ~GLIBInterface();

            //Configure the Glib w or w/o a GLIB object as parameter
            void ConfigureGlib(/*GLIB& pGLIB*/);
            //Start an acq
            void Start();
            //Stop an acq
            void Stop( uint32_t pNthAcq );
            //(Un)pause the acq
            void Pause();
            void Unpause();
            //Read Data from acq
            void ReadDAQ( uint32_t pNthAcq, bool pBreakTrigger );
            //Select the SRAM for DAQ
            void SRAMforDAQ( uint32_t pNthAcq );
    };
}

#endif
