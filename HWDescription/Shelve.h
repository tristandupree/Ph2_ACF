/*!

	\file 				Shelve.h
	\brief 				Shelve Description class, handles a vector of Board
	\author 			Lorenzo BIDEGAIN
	\version			1.0
	\date 				11/08/14
	Support : 			mail to : lorenzo.bidegain@cern.ch

*/

#ifndef Shelve_h__
#define Shelve_h__

#include "BeBoard.h"

/*!
* \namespace Ph2_HwDescription
* \brief Namespace regrouping all the hardware description
*/
namespace Ph2_HwDescription{

	/*!
	* \class Shelve
	* \brief contains a vector of Board which are connected to the Shelve
	*/
	class Shelve{

	public:

		//C'tor which takes the ShelveId
		Shelve( uint8_t pShelveId);

		// Default C'tor
		Shelve();

		// D'tor
		~Shelve(){};

		/*!
		* \brief Get the number of Board connected to the Shelve
		* \return The size of the vector
		*/
		uint8_t getNBoard(){return fBoardVector.size();};
		/*!
		* \brief Adding a Board to the vector
		* \param pBoard
		*/
		void addBoard( BeBoard& pBoard );
		/*!
		* \brief Remove a Board from the vector
		* \param pBeId
		* \return a bool which indicate if the removing was successful
		*/
		bool   removeBoard( uint8_t pBeId );
		/*!
		* \brief Get a Board from the vector
		* \param pBeId
		* \return a pointer of Board, so we can manipulate directly the Board contained in the vector
		*/
		BeBoard*   getBoard( uint8_t pBeId );

	public:
		uint8_t fShelveId;

	private:
		// Vector of Board
		std::vector< BeBoard > fBoardVector;

	};
}

#endif
