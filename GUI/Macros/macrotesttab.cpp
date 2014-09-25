//Simple place for a user to quickly add a macro and corresponding button

#include "macrotesttab.h"
#include "ui_macrotesttab.h"

#include "../HWDescription/Cbc.h"
#include "../HWDescription/Module.h"
#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../tools/Calibration.h"
#include "../HWDescription/Definition.h"
#include "../HWInterface/Utilities.h"
#include "../System/SystemController.h"
#include <boost/format.hpp>
#include <TH1F.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TApplication.h>
#include <TROOT.h>

namespace externalCalib {
#include "../src/calibrationtest.cc"
}

namespace externalInterface {
#include "../src/interfacetest.cc"
}

namespace externalReadData {
#include "../src/readdatatest2CBC.cc"
}

namespace externalSystemTest {
#include "../src/systemtest.cc"
}



namespace GUI{



    MacroTestTab::MacroTestTab(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::MacroTestTab)
    {
        ui->setupUi(this);
    }

    MacroTestTab::~MacroTestTab()
    {
        delete ui;
    }

    void MacroTestTab::on_btnCalib_clicked()
    {
        char* i[] = {"p"};
        externalCalib::main(1, i);

    }

    void MacroTestTab::on_btnInterface_clicked()
    {

        char* i[] = {"p"};
        externalInterface::main(1, i);

    }

    void MacroTestTab::on_btnReadData_clicked()
    {

        char* i[] = {"p"};
        externalReadData::main(1, i);


    }

    void MacroTestTab::on_btnSystemTest_clicked()
    {
        externalSystemTest::main();

    }

}


