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

#include "GlibFWInterface.h"

using namespace Ph2_HwDescription;

/*!
* \namespace Ph2_HwInterface
* \brief Namespace regrouping all the interfaces to the hardware
*/
namespace Ph2_HwInterface
{
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
            	BeBoardInterface(BeBoardFWInterface& pBoardFWI);
            	/*!
            	* \brief Destructor of the BeBoardInterface class
            	*/
            	~BeBoardInterface();

		/*!
            	* \brief Get the board infos
            	* \param pGlib
           	*/
            	void getBoardInfo(Glib& pGlib)
		{cBoardFWI.getBoardInfo(pGlib);};

		/*!
		* \brief Update both Glib register and Config File
            	* \param pGlib
            	* \param pRegNode : Node of the register to update
            	* \param pVal : Value to write
            	*/
            	void WriteReg(Glib& pGlib,const std::string& pRegNode,const uint32_t& pVal);
            	/*!
            	* \brief Update Config File with the value in the Glib register
            	* \param pGlib
            	* \param pRegNode : Node of the register to update
		*/
		void ReadReg(Glib& pGlib,const std::string& pRegNode);
		/*!
            	* \brief Configure the Glib with its Config File
            	* \param pGlib
            	*/
		void ConfigureGlib(Glib& pGlib)
		{setBoard(pGlib.getBeId());
		fBoardFW->ConfigureGlib(pGlib)};
		/*!
            	* \brief Detect the right FE Id to write the right registers (not tested)
            	*/
		void SelectFEId(Glib& pGlib)
		{setBoard(pGlib.getBeId());
		fBoardFW->SelectFEId();};
            	/*!
            	* \brief Start a DAQ
           	* \param pGlib
            	*/
            	void Start(Glib& pGlib)
		{setBoard(pGlib.getBeId());
		fBoardFW->Start(pGlib);};
            	/*!
            	* \brief Stop a DAQ
            	* \param pGlib
            	* \param pNthAcq : actual number of acquisitions
            	*/
            	void Stop(Glib& pGlib,uint32_t pNthAcq)
		{setBoard(pGlib.getBeId());
		fBoardFW->Stop(pGlib, pNthAcq);};
            	/*!
            	* \brief Pause a DAQ
            	* \param pGlib
            	*/
            	void Pause(Glib& pGlib)
		{setBoard(pGlib.getBeId());
		fBoardFW->Pause(pGlib);};
            	/*!
            	* \brief Unpause a DAQ
            	* \param pGlib
            	*/
            	void Unpause(Glib& pGlib)
		{setBoard(pGlib.getBeId());
		fBoardFW->Unpause(pGlib);};
            	/*!
            	* \brief Read data from DAQ
            	* \param pGlib
            	* \param pNthAcq : actual number of acquisitions
            	* \param pBreakTrigger : if true, enable the break trigger
            	*/
            	void ReadData(Glib& pGlib, uint32_t pNthAcq, bool pBreakTrigger)
		{setBoard(pGlib.getBeId());
		fBoardFW->ReadData(pGlib, pNthAcq, pBreakTrigger);};
		/*!
		* \brief Run a DAQ
    	        * \param pGlib
    	        */
            	void Run(Glib& pGlib)
		{setBoard(pGlib.getBeId());
		fBoardFW->Run(pGlib);};

	};
}

#endif
