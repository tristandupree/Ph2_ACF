#include "lib/CustomTQtWidget.h"
#include "datatesttab.h"
#include "ui_datatesttab.h"
#include "lib/TQtWidget.h"
#include <QDebug>
#include <memory>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QTabWidget>
#include "TCanvas.h"
#include "TH2F.h"

#include "utils.h"

namespace GUI {

    DataTestTab::DataTestTab(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::DataTestTab),
        m_tabMainCbc(new QTabWidget)
    {
        ui->setupUi(this);
        ui->loCbcBox->addWidget(m_tabMainCbc);
        setupCanvas(true);

        m_Vcth = 120;
        m_Events= 100;
    }

    DataTestTab::~DataTestTab()
    {
        qDebug() << "Destructing " << this;
        delete ui;
    }

    void DataTestTab::setupCanvas(const bool cbc2)
    {
        int cNCbc;
        if(cbc2) cNCbc = 2;
        else cNCbc = 8;
        /*if(m_tabMainCbc->count() != m_vectorCanvasOccupancy.size())
        {
            qDebug() << "ERROR! Canvas size does not match tab size";
            qDebug() << "Cbc Tabs: " << m_tabMainCbc->count();
            qDebug() << "Canvas size : " << m_vectorCanvasOccupancy.size();
        }*/

        if(m_tabMainCbc->count() > 0)
        {
            m_tabMainCbc->clear();
            m_vecTWidget.clear();
        }
        m_vecTWidget.push_back(new TQtWidget(this));
        //m_canvasOccupy = m_vecTWidget.at(0)->GetCanvas();
        //m_canvasOccupy->Divide(2);
        QString title = QString("Occupancy");
        m_tabMainCbc->addTab(createCbcTab(), title);


        /*TCanvas *c1 = m_vectorCanvasOccupancy.at(0)->GetCanvas();

        c1->Divide(2,2,0,0);
        TH2F *h1 = new TH2F("h1","test1",10,0,1,20,0,20);
        TH2F *h2 = new TH2F("h2","test2",10,0,1,20,0,100);
        TH2F *h3 = new TH2F("h3","test3",10,0,1,20,-1,1);
        TH2F *h4 = new TH2F("h4","test4",10,0,1,20,0,1000);

        c1->cd(1);
        gPad->SetTickx(2);
        h1->Draw();

        c1->cd(2);
        gPad->SetTickx(2);
        gPad->SetTicky(2);
        h2->GetYaxis()->SetLabelOffset(0.01);
        h2->Draw();

        c1->cd(3);
        h3->Draw();

        c1->cd(4);
        gPad->SetTicky(2);
        h4->Draw();*/

    }

    QTabWidget *DataTestTab::createCbcTab()
    {
        QTabWidget *tab = new QTabWidget;
        QHBoxLayout *loH = new QHBoxLayout;

        loH->addWidget(m_vecTWidget.at(0));
        //m_vectorLayout.push_back(loH);

        tab->setLayout(loH);
        return tab;
    }

    void DataTestTab::drawOccupancy(const std::vector<std::shared_ptr<TH1F> > hists)
    {
        for (int i=0; i<2; i++)
        {
            m_vecHistOccupancy = hists;
            TQtWidget *u = new TQtWidget(this); //No idea why this helps flush
            m_vecTWidget.at(0)->GetCanvas()->cd();
            //m_canvasOccupy->cd(i);
            //m_vecTWidget.at(0)->cd();
            m_vecHistOccupancy.at(i)->Draw();
            m_vecTWidget.at(0)->Refresh();
        }
    }

    void DataTestTab::getVcthDialValue()
    {
        emit sendVcthValue(m_Vcth);
    }

    void DataTestTab::getEventsDial()
    {
        emit sendEventsNumber(m_Events);
    }

    void DataTestTab::getIsRegTestChecked()
    {
        emit sendIsRegTestChecked(ui->checkTestReg->isChecked());
    }

    void DataTestTab::getIsScanChecked()
    {
        emit sendIsScanChecked(ui->checkScan->isChecked());
    }

    void DataTestTab::onDataTestStart()
    {
        ui->btnStart->setEnabled(false);
    }

    void DataTestTab::onDataTestFinish()
    {
        ui->btnStart->setEnabled(true);
    }

    void DataTestTab::on_btnStart_clicked()
    {
        emit notifyAddGraph();
    }

    void DataTestTab::on_dialVcth_sliderMoved(int position)
    {
        ui->txtVcth->setText(QString::number(position));
        m_Vcth = position;
    }

    void DataTestTab::on_txtVcth_editingFinished()
    {
        int value = (ui->txtVcth->text()).toInt();
        if(value<256)
        {
            ui->dialVcth->setValue(value);
            m_Vcth = value;
        }
        else
        {
            ui->txtVcth->setText(QString("0"));
            ui->dialVcth->setValue(0);
            m_Vcth = value;
        }
    }

    void DataTestTab::on_dialEvents_sliderMoved(int position)
    {
        ui->txtEvents->setText(QString::number(position));
        m_Events = position;
    }

    void DataTestTab::on_txtEvents_editingFinished()
    {
        int value = (ui->txtVcth->text()).toInt();
        if(value<501)
        {
            ui->dialVcth->setValue(value);
            m_Vcth = value;
        }
        else
        {
            ui->txtVcth->setText(QString("0"));
            ui->dialVcth->setValue(0);
            m_Vcth = value;
        }
    }

}
