#include "mainview.h"
#include "ui_mainview.h"
#include "View/setuptab.h"
#include "View/cbcregisterstab.h"
#include "View/datatesttab.h"

#include <QDebug>

namespace GUI{

    MainView::MainView(QWidget *parent,
                       SetupTab& setup,
                       CbcRegistersTab& regTab,
                       DataTestTab& data) :
        QMainWindow(parent),
        ui(new Ui::MainView)
    {
        ui->setupUi(this);
        ui->loSetup->addWidget(&setup);
        ui->loRegisters->addWidget(&regTab);
        ui->loDataTest->addWidget(&data);
        //ui->loGraphs->addWidget(&vcth);
    }

    MainView::~MainView()
    {
        qDebug() << "Destructing " << this;
        delete ui;
    }
}
