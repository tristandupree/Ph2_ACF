/*!

        \file                                    Visitor.h
        \brief                                  Generic Visitor Class
        \author                                Georg AUZINGER
        \version                               1.0
        \date                                   07/10/14
        Support :                              mail to : georg.auzinger@cern.ch
 */


#ifndef Visitor_h__
#define Visitor_h__

#include <cstdint>

namespace Ph2_System
{
	class SystemController;
}

namespace Ph2_HwDescription
{
	class Shelve;
	class BeBoard;
	class Module;
	class Cbc;
}

class HwDescriptionVisitor
{
  public:
	/*!
	 * \brief Visitor for top level System Controller
	 * \param pSystemController
	 */
	virtual void visit( Ph2_System::SystemController& pSystemController ) = 0;
	/*!
	 * \brief Visitor for Shelve Class
	 * \param pShelve
	 */
	virtual void visit( Ph2_HwDescription::Shelve& pShelve ) = 0;
	/*!
	 * \brief Visitor for BeBoard Class
	 * \param pBeBoard
	 */
	virtual void visit( Ph2_HwDescription::BeBoard& pBeBoard ) = 0;
	/*!
	 * \brief Visitor for Module Class
	 * \param pModule
	 */
	virtual void visit( Ph2_HwDescription::Module& pModule ) = 0;
	/*!
	 * \brief Visitor for Cbc Class
	 * \param pCbc
	 */
	virtual void visit( Ph2_HwDescription::Cbc& pCbc ) = 0;
};


#endif