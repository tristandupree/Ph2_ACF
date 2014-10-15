#include "mainview.h"
#include "ui_mainview.h"
#include "View/setuptab.h"
#include "View/cbcregisterstab.h"
#include "View/datatesttab.h"
#include "View/aboutbox.h"

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

        enableAllTabsSlot(false);
    }

    MainView::~MainView()
    {
        qDebug() << "Destructing " << this;
        delete ui;
    }

    void MainView::enableAllTabsSlot(const bool enable)
    {
        ui->tabRegisters->setEnabled(enable);
        ui->tabDataTest->setEnabled(enable);
        ui->tabMacroTest->setEnabled(enable);
    }

    void MainView::on_actionAbout_triggered()
    {
        AboutBox about_box;
        about_box.show();
        about_box.exec();
    }

    void MainView::on_actionUser_Settings_triggered()
    {

    }


}
