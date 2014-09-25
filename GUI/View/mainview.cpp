#include "mainview.h"
#include "ui_mainview.h"
#include "View/setuptab.h"
#include "View/cbcregisterstab.h"
#include "View/datatesttab.h"

#include "Macros/macrotesttab.h"

#include <QDebug>

namespace GUI{

    MainView::MainView(QWidget *parent,
                       SetupTab& setup,
                       CbcRegistersTab& regTab,
                       DataTestTab& data,
                       MacroTestTab& macroTest) :
        QMainWindow(parent),
        ui(new Ui::MainView)
    {
        ui->setupUi(this);
        ui->loSetup->addWidget(&setup);
        ui->loRegisters->addWidget(&regTab);
        ui->loDataTest->addWidget(&data);
        ui->loMacroTest->addWidget(&macroTest);
        //ui->loGraphs->addWidget(&vcth);
    }

    MainView::~MainView()
    {
        qDebug() << "Destructing " << this;
        delete ui;
    }
}
