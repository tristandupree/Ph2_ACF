#include "datatesttab.h"
#include "ui_datatesttab.h"
#include "lib/qcustomplot.h"

namespace GUI {

    DataTestTab::DataTestTab(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::DataTestTab)
    {
        ui->setupUi(this);
    }

    DataTestTab::~DataTestTab()
    {
        qDebug() << "Destructing " << this;
        delete ui;
    }
    void DataTestTab::setGraphs(QCPBars* plot)
    {
       // plot->


        ui->graphDataTest->addPlottable(plot);
    }

    void DataTestTab::on_btnStart_clicked()
    {

        emit notifyAddGraph();

    }
}


