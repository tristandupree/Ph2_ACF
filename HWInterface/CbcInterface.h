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
		void setBoard(uint8_t pBoardId);


	public:

		/*!
		* \brief Constructor of the CbcInterface class
		* \param Reference to a Map of BoardId vs pointer to the type of Board
		*/
		CbcInterface(BeBoardFWMap& pBoardMap);
		/*!
		* \brief Destructor of the CbcInterface class
		*/
		~CbcInterface();

    	/*!
    	* \brief Configure the Cbc with the Cbc Config File
    	* \param pCbc
    	*/
    	void ConfigureCbc(Cbc* pCbc);
    	/*!
    	* \brief Write the designated register in Cbc
    	* \param pCbc
    	* \param pRegNode : Node of the register to write
    	* \param pValue : Value to write
    	*/
    	void WriteCbc(Cbc* pCbc, const std::string& pRegNode, uint8_t pValue);
    	/*!
    	* \brief Read the designated register in the Cbc and update the Cbc Config File
    	* \param pCbc
    	* \param pRegNode : Node of the register to read
    	*/
    	void UpdateCbc(Cbc* pCbc,const std::string& pRegNode);
    	/*!
    	* \brief Read same register in all Cbcs and then UpdateCbc
    	* \param pModule : Module containing vector of Cbcs
    	* \param pRegNode : Node of the register to read
    	*/
    	void UpdateAllCbc(Module* pModule,const std::string& pRegNode);
    	/*!
    	* \brief Write same register in all Cbcs and then UpdateCbcRead
    	* \param pModule : Module containing vector of Cbcs
    	* \param pRegNode : Node of the register to write
    	* \param pValue : Value to write
    	*/
    	void WriteBroadcast(Module* pModule,const std::string& pRegNode,uint8_t pValue);
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
		{fBoardFW->EncodeReg(pRegItem, pCbcId, pVecReq);}; /*!< Encode a/several word(s) readable for a Cbc*/
		/*!
		* \brief Decode a word from a read of a register of the Cbc
    	* \param pRegItem : RegItem containing infos (name, adress, value...) about the register to read
    	* \param pCbcId : Id of the Cbc to work with
    	* \param pWord : variable to put the decoded word
    	*/
    	void DecodeReg(CbcRegItem& pRegItem, uint8_t& pCbcId, uint32_t pWord)
		{fBoardFW->DecodeReg(pRegItem, pCbcId, pWord);}; /*!< Decode a word from a read of a register of the Cbc*/

	};
}

#endif
