/*!
*
* \file BeBoardInterface.h
* \brief BeBoardInterface User interface for the board
* \author Lorenzo BIDEGAIN, Nicolas Pierre
* \date 30/07/14
*
* Support : mail to : lorenzo.bidegain@cern.ch, nicolas.pierre@cern.ch
*
*/

#ifndef __BEBOARDINTERFACE_H__
#define __BEBOARDINTERFACE_H__

#include "BeBoardFWInterface.h"

using namespace Ph2_HwDescription;

/*!
* \namespace Ph2_HwInterface
* \brief Namespace regrouping all the interfaces to the hardware
*/
namespace Ph2_HwInterface
{

	typedef std::map<int8_t,BeBoardFWInterface*> BeBoardFWMap;

	/*!
	* \class BeBoardInterface
	* \brief 
	*/
	class BeBoardInterface {

	private:
		BeBoardFWMap& fBoardMap;
		BeBoardFWInterface* fBoardFW;
		uint8_t prevBoardId;

	private:
		void setBoard(uint8_t pBoardId);

	public:
		/*!
            	* \brief Constructor of the BeBoardInterface class
            	* \param Reference to the BoardFWInterface
            	*/
            	BeBoardInterface(BeBoardFWMap& pBoardMap);
            	/*!
            	* \brief Destructor of the BeBoardInterface class
            	*/
            	~BeBoardInterface();

		/*!
            	* \brief Get the board infos
            	* \param pBoard
           	*/
            	void getBoardInfo(BeBoard* pBoard)
		{setBoard(pBoard->getBeId());
		fBoardFW->getBoardInfo(pBoard);};

		/*!
		* \brief Update both Board register and Config File
            	* \param pBoard
            	* \param pRegNode : Node of the register to update
            	* \param pVal : Value to write
            	*/
            	void WriteBoardReg(BeBoard* pBoard,const std::string& pRegNode,const uint32_t& pVal);
            	/*!
            	* \brief Update Config File with the value in the Board register
            	* \param pBoard
            	* \param pRegNode : Node of the register to update
		*/
		void ReadBoardReg(BeBoard* pBoard,const std::string& pRegNode);
		/*!
            	* \brief Configure the board with its Config File
            	* \param pBoard
            	*/
		void ConfigureBoard(BeBoard* pBoard)
		{setBoard(pBoard->getBeId());
		fBoardFW->ConfigureBoard(pBoard);};
            	/*!
            	* \brief Start a DAQ
           	* \param pBoard
            	*/
            	void Start(BeBoard* pBoard)
		{setBoard(pBoard->getBeId());
		fBoardFW->Start(pBoard);};
            	/*!
            	* \brief Stop a DAQ
            	* \param pBoard
            	* \param pNthAcq : actual number of acquisitions
            	*/
            	void Stop(BeBoard* pBoard,uint32_t pNthAcq)
		{setBoard(pBoard->getBeId());
		fBoardFW->Stop(pBoard, pNthAcq);};
            	/*!
            	* \brief Pause a DAQ
            	* \param pBoard
            	*/
            	void Pause(BeBoard* pBoard)
		{setBoard(pBoard->getBeId());
		fBoardFW->Pause(pBoard);};
            	/*!
            	* \brief Unpause a DAQ
            	* \param pBoard
            	*/
            	void Resume(BeBoard* pBoard)
		{setBoard(pBoard->getBeId());
		fBoardFW->Resume(pBoard);};
            	/*!
            	* \brief Read data from DAQ
            	* \param pBoard
            	* \param pNthAcq : actual number of acquisitions
            	* \param pBreakTrigger : if true, enable the break trigger
            	*/
            	void ReadData(BeBoard* pBoard, uint32_t pNthAcq, bool pBreakTrigger)
		{setBoard(pBoard->getBeId());
		fBoardFW->ReadData(pBoard, pNthAcq, pBreakTrigger);};

	};
}

#endif
