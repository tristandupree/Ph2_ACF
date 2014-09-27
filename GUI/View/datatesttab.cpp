#include "datatesttab.h"
#include "ui_datatesttab.h"
#include "lib/qcustomplot.h"

namespace GUI {

    DataTestTab::DataTestTab(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::DataTestTab)

    {
        ui->setupUi(this);
        setupCanvas();
    }

    DataTestTab::~DataTestTab()
    {
        qDebug() << "Destructing " << this;
        delete ui;
    }

    void DataTestTab::setupCanvas()
    {
        m_dataTestHist = new QCPBars(ui->graphDataTest->xAxis, ui->graphDataTest->yAxis);
        ui->graphDataTest->xAxis->setLabel("x");
        ui->graphDataTest->yAxis->setLabel("y");

        ui->graphDataTest->addPlottable(m_dataTestHist);
        ui->graphDataTest->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    }

    void DataTestTab::drawGraph(const QVector<double> &x, const QVector<double> &y)
    {
        auto customPlot = ui->graphDataTest;

        m_dataTestHist->clearData();

        m_dataTestHist->setData(x,y);
        customPlot->rescaleAxes();
        customPlot->replot();

    }

    void DataTestTab::on_btnStart_clicked()
    {

        emit notifyAddGraph();

    }
}


