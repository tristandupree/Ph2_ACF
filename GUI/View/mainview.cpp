#include "mainview.h"
#include "ui_mainview.h"
#include "View/setuptab.h"
#include "View/cbcregisterstab.h"
#include "View/calibratetab.h"
#include "View/hybridtesttab.h"
#include "View/cmtesttab.h"
#include "View/aboutbox.h"
#include "View/tbrowsertab.h"

#include <QDebug>

namespace GUI{

    MainView::MainView(QWidget *parent,
                       SetupTab& setup,
                       CbcRegistersTab& regTab,
                       HybridTestTab& hybrid,
                       CalibrateTab& calibrate,
                       CmTestTab& cm,
                       TBrowserTab& brow) :
        QMainWindow(parent),
        ui(new Ui::MainView)
    {
        ui->setupUi(this);
        setWindowTitle("Ph2_ACF");
        ui->loSetup->addWidget(&setup);
        ui->loRegisters->addWidget(&regTab);
        ui->loHybridTest->addWidget(&hybrid);
        ui->loHybridTest->addWidget(&calibrate);
        ui->loHybridTest->addWidget(&cm);
        //ui->loCalibrate->addWidget(&calibrate);
        ui->loTBrowser->addWidget(&brow);

        //enableAllTabsSlot(false);
    }

    MainView::~MainView()
    {
        qDebug() << "Destructing " << this;
        delete ui;
    }

    void MainView::enableAllTabsSlot(const bool enable)
    {
        ui->tabRegisters->setEnabled(enable);
        ui->tabHybridTest->setEnabled(enable);
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
