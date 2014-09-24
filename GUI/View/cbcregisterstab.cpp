#include "cbcregisterstab.h"
#include "ui_cbcregisterstab.h"

#include <QVector>

#include "../HWDescription/Cbc.h"
#include "../HWDescription/Module.h"
#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../HWInterface/Utilities.h"
#include "../Model/systemcontroller.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;
//using namespace Ph2_System;

namespace GUI {

    CbcRegistersTab::CbcRegistersTab(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::CbcRegistersTab)
    {
        ui->setupUi(this);
        startup();
    }

    CbcRegistersTab::~CbcRegistersTab()
    {
        qDebug() << "Destructing " << this;
        delete ui;
    }

    void CbcRegistersTab::startup()
    {
    }
}
