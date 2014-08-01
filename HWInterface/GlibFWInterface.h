/*!
*
* \file GlibFWInterface.h
* \brief GlibFWInterface init/config of the Glib and its Cbc's
* \author Lorenzo BIDEGAIN, Nicolas Pierre
* \date 28/07/14
*
* Support : mail to : lorenzo.bidegain@cern.ch, nicolas.pierre@cern.ch
*
*/

#ifndef __GLIBFWINTERFACE_H__
#define __GLIBFWINTERFACE_H__

#include <string>
#include <map>
#include <vector>
#include "BeBoardFWInterface.h"
#include <TROOT.h>
#include <limits.h>
#include <boost/cstdint.hpp>


using namespace Ph2_HwDescription;

/*!
* \namespace Ph2_HwInterface
* \brief Namespace regrouping all the interfaces to the hardware
*/
namespace Ph2_HwInterface
{
	/*!
	* \class GlibFWInterface
	* \brief Permit r/w given registers in the Glib you specify
	*/
	class GlibFWInterface : public BeBoardFWInterface {

	private:
		struct timeval fStartVeto;
            	std::string fStrSram, fStrSramUserLogic, fStrFull, fStrReadout, fStrOtherSram;
            	std::string fCbcStubLat, fCbcI2CCmdAck, fCbcI2CCmdRq, fCbcHardReset, fCbcFastReset;


	private:
            	/*!
            	* \brief SRAM selection for DAQ
            	* \param pNthAcq : actual number of acquisitions
           	*/
		void SelectSRAM(uint32_t pNthAcq);

	public:
		/*!
		* \brief Constructor of the GlibFWInterface class
		* \param puHalConfigFileName : path of the uHal Config File
		*/
		GlibFWInterface(const char *puHalConfigFileName);
		/*!
		* \brief Destructor of the GlibFWInterface class
		*/
		~GlibFWInterface(){};

		/*!
            	* \brief Configure the Glib with its Config File
            	* \param pGlib
            	*/
            	void ConfigureGlib(Glib& pGlib);
		/*!
            	* \brief Detect the right FE Id to write the right registers (not tested)
            	*/
		void SelectFEId();
            	/*!
            	* \brief Start a DAQ
           	* \param pGlib
            	*/
            	void Start(Glib& pGlib);
            	/*!
            	* \brief Stop a DAQ
            	* \param pGlib
            	* \param pNthAcq : actual number of acquisitions
            	*/
            	void Stop(Glib& pGlib,uint32_t pNthAcq);
            	/*!
            	* \brief Pause a DAQ
            	* \param pGlib
            	*/
            	void Pause(Glib& pGlib);
            	/*!
            	* \brief Unpause a DAQ
            	* \param pGlib
            	*/
            	void Unpause(Glib& pGlib);
            	/*!
            	* \brief Read data from DAQ
            	* \param pGlib
            	* \param pNthAcq : actual number of acquisitions
            	* \param pBreakTrigger : if true, enable the break trigger
            	*/
            	void ReadData(Glib& pGlib, uint32_t pNthAcq, bool pBreakTrigger);
		/*!
		* \brief Run a DAQ
    	        * \param pGlib
    	        */
            	void Run(Glib& pGlib);

	//Methods for the Cbc's:

	private: 

		//I2C Methods

		/*!
		* \brief Wait for the I2C command acknowledgement
            	* \param pAckVal : Expected status of acknowledgement, 1/0 -> true/false
            	* \param pNcount : Number of registers at stake
            	* \return boolean confirming the acknowledgement
            	*/
    		bool I2cCmdAckWait( uint32_t pAckVal, uint8_t pNcount=1 );
            	/*!
            	* \brief Send request to r/w blocks via I2C
            	* \param pVecReq : Block of words to send
            	* \param pWrite : 1/0 -> Write/Read
            	*/
            	void SendBlockCbcI2cRequest(std::vector<uint32_t>& pVecReq, bool pWrite);
            	/*!
            	* \brief Read blocks from SRAM via I2C
            	* \param pVecReq : Vector to stack the read words
            	*/
            	void ReadI2cBlockValuesInSRAM(std::vector<uint32_t> &pVecReq );
            	/*!
            	* \brief Enable I2C communications
            	* \param pCbc : Cbc to work with
            	* \param pEnable : 1/0 -> Enable/Disable
            	*/
            	void EnableI2c( Cbc* pCbc, bool pEnable );

	public:

            	//r/w the Cbc registers
            	/*!
            	* \brief Write register blocks of a Cbc
            	* \param pCbc : Cbc to work with
            	* \param pVecReq : Block of words to write
            	*/
            	void WriteCbcBlockReg( Cbc* pCbc, std::vector<uint32_t>& pVecReq );
            	/*!
            	* \brief Read register blocks of a Cbc
            	* \param pCbc : Cbc to work with
            	* \param pVecReq : Vector to stack the read words
            	*/
            	void ReadCbcBlockReg( Cbc* pCbc, std::vector<uint32_t>& pVecReq );

    };
}

#endif
