#ifndef COMMONVISITORS_H__
#define COMMONVISITORS_H__

#include "../HWInterface/BeBoardFWInterface.h"
#include "../HWDescription/Shelve.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../HWInterface/Visitor.h"

#include <iostream>
#include <vector>
#include <stdlib.h>
# include <string>



// wriite single reg
struct CbcRegWriter : public HwDescriptionVisitor
{
	Ph2_HwInterface::CbcInterface* fInterface;
	std::string fRegName;
	uint8_t fRegValue;

	CbcRegWriter( Ph2_HwInterface::CbcInterface* pInterface, std::string pRegName, uint8_t pRegValue ): fInterface( pInterface ), fRegName( pRegName ), fRegValue( pRegValue ) {}

	void visit( Ph2_HwDescription::Cbc& pCbc ) {
		fInterface->WriteCbcReg( &pCbc, fRegName, fRegValue );
	}
};

//write multi reg
struct CbcMultiRegWriter : public HwDescriptionVisitor
{
	Ph2_HwInterface::CbcInterface* fInterface;
	std::vector<std::pair<std::string, uint8_t>> fRegVec;

	CbcMultiRegWriter( Ph2_HwInterface::CbcInterface* pInterface, std::vector<std::pair<std::string, uint8_t>> pRegVec ): fInterface( pInterface ), fRegVec( pRegVec ) {}

	void visit( Ph2_HwDescription::Cbc& pCbc ) {
		fInterface->WriteCbcMultReg( &pCbc, fRegVec );
	}
};

// HwDescription Objects Counter
class Counter : public HwDescriptionVisitor
{
  private:
	uint32_t fNCbc = 0;
	uint32_t fNFe = 0;
	uint32_t fNBe = 0;
	uint32_t fNShelve = 0;
  public:
	void visit( Ph2_HwDescription::Cbc& pCbc ) {
		fNCbc++;
	}
	void visit( Ph2_HwDescription::Module& pModule ) {
		fNFe++;
	}
	void visit( Ph2_HwDescription::BeBoard& pBoard ) {
		fNBe++;
	}
	void visit( Ph2_HwDescription::Shelve& pShelve ) {
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
		return fNShelve;
	};
};

// Configurator
class Configurator: public HwDescriptionVisitor
{
  private:
	Ph2_HwInterface::BeBoardInterface* fBeBoardInterface;
	Ph2_HwInterface::CbcInterface* fCbcInterface;
  public:
	Configurator( Ph2_HwInterface::BeBoardInterface* pBeBoardInterface, Ph2_HwInterface::CbcInterface* pCbcInterface ): fBeBoardInterface( pBeBoardInterface ), fCbcInterface( pCbcInterface ) {}
	void visit( BeBoard& pBoard ) {
		fBeBoardInterface->ConfigureBoard( &pBoard );
		std::cout << "Successfully configured Board " << int( pBoard.getBeId() ) << std::endl;
	}
	void visit( Cbc& pCbc ) {
		fCbcInterface->ConfigureCbc( &pCbc );
		std::cout << "Successfully configured Cbc " << int( pCbc.getCbcId() ) << std::endl;

	}
};



#endif
