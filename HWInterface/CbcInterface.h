/*!
*
* \file CBCInterface.h
* \brief CBCInterface.cc User interface for the cbc's
* \author Lorenzo BIDEGAIN, Nicolas Pierre
* \date 30/07/14
*
* Support : mail to : lorenzo.bidegain@cern.ch, nicolas.pierre@cern.ch
*
*/

#ifndef __CBCINTERFACE_H__
#define __CBCINTERFACE_H__

#include <vector>
#include "GlibFWInterface.h"

using namespace Ph2_HwDescription;

/*!
* \namespace Ph2_HwInterface
* \brief Namespace regrouping all the interfaces to the hardware
*/
namespace Ph2_HwInterface
{

	typedef std::map<int8_t,BeBoardFWInterface*> BeBoardFWMap;

	/*!
	* \class CBCInterface
	* \brief
	*/
	class CbcInterface {

	private:
		BeBoardFWMap& fBoardMap;
		BeBoardFWInterface* fBoardFW;
		uint8_t prevBoardId;

	private:
		/*!
		* \brief Choose the board to talk with
		* \param pBoardId
		*/
		void setBoard(uint8_t pBoardId);

	public:
		/*!
		* \brief Constructor of the CBCInterface Class
		* \param pBoardMap
		*/
		CbcInterface(BeBoardFWMap& pBoardMap);
		/*!
		* \brief Destructor of the CBCInterface Class
		*/
		~CbcInterface();
        /*!
        * \brief Configure the Cbc with the Cbc Config File
        * \param pCbc
        */
        void ConfigureCbc(Cbc* pCbc);
        /*!
        * \brief Write the designated register in both Cbc and Cbc Config File
        * \param pCbc
        * \param pRegNode : Node of the register to write
        * \param pValue : Value to write
        */
        void WriteCbcReg(Cbc* pCbc, const std::string& pRegNode, uint32_t pValue);
		/*!
		* \brief Write the designated register in both Cbc and Cbc Config File
		* \param pCbc
		* \param pRegNode : Node of the register to write
		* \param pValue : Value to write
		*/
		void WriteCbcMultReg(Cbc* pCbc, std::vector< std::pair<std::string,uint32_t> > pVecReq);
		/*!
		* \brief Write same register in all Cbcs and then UpdateCbc (not working with current Firmware)
		* \param pModule : Module containing vector of Cbcs
		* \param pRegNode : Node of the register to write
		* \param pValue : Value to write
		*/
		void WriteBroadcast(Module* pModule,const std::string& pRegNode,uint32_t pValue);
        /*!
        * \brief Read the designated register in the Cbc
        * \param pCbc
        * \param pRegNode : Node of the register to read
        */
        void ReadCbcReg(Cbc* pCbc,const std::string& pRegNode);
		/*!
		* \brief Read the designated register in the Cbc
		* \param pCbc
		* \param pRegNode : Node of the register to read
		*/
		void ReadCbcMultReg(Cbc* pCbc,std::vector<std::string> pVecReg);
        /*!
        * \brief Read all register in all Cbcs and then UpdateCbc
        * \param pModule : Module containing vector of Cbcs
        */
        void ReadAllCbc(Module* pModule);
        /*!
        * \brief Hard reset of the Cbc
        * \param pCbc
        */
        void CbcHardReset(Cbc* pCbc);
        /*!
        * \brief Fast Reset of the Cbc
        * \param pCbc
        */
        void CbcFastReset(Cbc* pCbc);

		//Encode/Decode Cbc values
		/*!
		* \brief Encode a/several word(s) readable for a Cbc
		* \param pRegItem : RegItem containing infos (name, adress, value...) about the register to write
		* \param pCbcId : Id of the Cbc to work with
		* \param pVecReq : Vector to stack the encoded words
		*/
		void EncodeReg(CbcRegItem& pRegItem, uint8_t& pCbcId, std::vector<uint32_t>& pVecReq)
		{fBoardFW->EncodeReg(pRegItem, pCbcId, pVecReq);};
		/*!
		* \brief Decode a word from a read of a register of the Cbc
    	* \param pRegItem : RegItem containing infos (name, adress, value...) about the register to read
    	* \param pCbcId : Id of the Cbc to work with
    	* \param pWord : variable to put the decoded word
    	*/
    	void DecodeReg(CbcRegItem& pRegItem, uint8_t& pCbcId, uint32_t pWord)
		{fBoardFW->DecodeReg(pRegItem, pCbcId, pWord);};

    };
}

#endif
