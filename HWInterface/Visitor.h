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

#include <stdint.h>

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
	virtual void visit( Ph2_System::SystemController& pSystemController ) {}
	// virtual void visit() = 0;

	/*!
	 * \brief Visitor for Shelve Class
	 * \param pShelve
	 */
	virtual void visit( Ph2_HwDescription::Shelve& pShelve ) {}
	/*!
	 * \brief Visitor for BeBoard Class
	 * \param pBeBoard
	 */
	virtual void visit( Ph2_HwDescription::BeBoard& pBeBoard ) {}
	/*!
	 * \brief Visitor for Module Class
	 * \param pModule
	 */
	virtual void visit( Ph2_HwDescription::Module& pModule ) {}
	/*!
	 * \brief Visitor for Cbc Class
	 * \param pCbc
	 */
	virtual void visit( Ph2_HwDescription::Cbc& pCbc ) {}
};

// some common visitors


// wriite single reg
struct CbcRegWriter : public HwDescriptionVisitor
{
	CbcInterface* fInterface;
	std::string fRegName;
	uint8_t fRegValue;

	CbcRegWriter( CbcInterface* pInterface, std::string pRegName, uint8_t pRegValue ): fInterface( pInterface ), fRegName( pRegName ), fRegValue( pRegValue ) {}

	void visit( Cbc& pCbc ) {
		pInterface->WriteCbcReg( &pCbc, fRegName, fRegValue );
	}
};

//write multi reg
struct CbcMultiRegWriter : public HwDescriptionVisitor
{
	CbcInterface* fInterface;
	std::vector<std::pair<std::string, uint8_t>> fRegVec;

	CbcRegWriter( CbcInterface* pInterface, std::vector<std::pair<std::string, uint8_t>> pRegVec; ): fInterface( pInterface ), fRegVec( pRegVec ) {}

	void visit( Cbc& pCbc ) {
		pInterface->WriteCbcReg( &pCbc, fRegVec );
	}
};

//HwDescription Objects
class Counter : public HwDescriptionVisitor
{
  private:
	uint32_t fNCbc = 0;
	uint32_t fNFe = 0;
	uint32_t fNBe = 0;
	uint32_t fNShelve = 0;
  public:
	void visit( Cbc& pCbc ) {
		fNCbc++;
	}
	void visit( Module& pModule ) {
		fNFe++;
	}
	void visit( BeBoard& pBoard ) {
		fNBe++;
	}
	void visit( Shelve& pShelve ) {
		fNShelve++;
	}
	uint32_t getNCbc() {
		return fNCbc;
	}
	uint32_t getNFe() {
		return fNFe;
	}
	uint32_t getNBe() {
		return fNBe;
	}
	uint32_t getNShelves() {
		return fNShelves
	};
};





#endif