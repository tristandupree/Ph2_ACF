#include "datatesttab.h"

#include "lib/CustomTQtWidget.h"
#include "ui_datatesttab.h"
#include "lib/TQtWidget.h"
#include <QDebug>
#include <memory>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QTabWidget>
#include "TCanvas.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TH1.h"

#include "utils.h"

using namespace Ph2_HwDescription;

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
        gStyle->SetOptStat( 000000 );
        gStyle->SetTitleOffset( 1.3, "Y" );

        int cNCbc;
        if(cbc2) cNCbc = 2;
        else cNCbc = 8;

        qDebug() << "setupCanvas";
        /*if(m_tabMainCbc->count() != m_vectorCanvasOccupancy.size())
        {
            qDebug() << "ERROR! Canvas size does not match tab size";
            qDebug() << "Cbc Tabs: " << m_tabMainCbc->count();
            qDebug() << "Canvas size : " << m_vectorCanvasOccupancy.size();
        }*/

        if(m_tabMainCbc->count() > 0)
        {
            m_tabMainCbc->clear();
            m_vecTWidget_Occupancy.clear();
            m_vecTWidget_Threshold.clear();
        }
        //m_canvasOccupy = m_vecTWidget_Occupancy.at(0)->GetCanvas();
        //m_canvasOccupy->Divide(2);
        QString titleOcc = QString("Occupancy");
        m_tabMainCbc->addTab(createOccupancyTab(), titleOcc);

        QString titleThresh = QString("Threshold Scan");
        m_tabMainCbc->addTab(createThresholdTab(), titleThresh);
    }

    QTabWidget *DataTestTab::createOccupancyTab()
    {
        QTabWidget *tab = new QTabWidget;
        QHBoxLayout *loH = new QHBoxLayout;

        for (int i=0; i<2 ;i++) //TODO support 8CBC
        {
            m_vecTWidget_Occupancy.push_back(new TQtWidget(this));
            loH->addWidget(m_vecTWidget_Occupancy.at(i));
            m_vectorLayout.push_back(loH); //incase I want to access later
            tab->setLayout(loH);
        }
        return tab;
    }

    QTabWidget *DataTestTab::createThresholdTab()
    {
        QTabWidget *tab = new QTabWidget;
        QHBoxLayout *loH = new QHBoxLayout;

        //for (int i=0; i<2 ;i++) //TODO support 8CBC
        //{
        m_vecTWidget_Threshold.push_back(new TQtWidget(this));
        loH->addWidget(m_vecTWidget_Threshold.at(0));
        m_vectorLayout.push_back(loH); //incase I want to access later
        tab->setLayout(loH);
        //}
        return tab;
    }

    void DataTestTab::drawOccupancy(const std::vector<std::shared_ptr<TH1F> > hists)
    {
        for (int i=0; i<2; i++)
        {
            m_vecTWidget_Occupancy.at(i)->Clear();
            m_vecHistOccupancy = hists;
            m_vecTWidget_Occupancy.at(i)->GetCanvas()->cd();
            m_vecHistOccupancy.at(i)->Draw();
            m_vecTWidget_Occupancy.at(i)->Refresh();
        }
    }

    void DataTestTab::drawThreshold(const std::vector<std::shared_ptr<TH1F> > hists, std::string option)
    {
        m_vecTWidget_Threshold.at(0)->Clear();
        m_vecHistThreshold = hists;
        m_vecTWidget_Threshold.at(0)->GetCanvas()->cd();
        m_vecHistThreshold.at(0)->Draw(option.c_str());
        m_vecTWidget_Threshold.at(0)->Refresh();
    }

    void DataTestTab::drawFitThreshold(const std::vector<std::shared_ptr<TF1> > graph, std::string option)
    {
        //m_vecTWidget_Threshold.at(0)->Clear();
        TQtWidget *flush = new TQtWidget(this); // no idea why this flushes
        m_vecFitThreshold = graph;
        m_vecTWidget_Threshold.at(0)->GetCanvas()->cd();
        m_vecHistThreshold.at(0)->Draw(option.c_str());
        m_vecTWidget_Threshold.at(0)->Refresh();
    }

    void DataTestTab::receiveRefreshHists()
    {
        int i=0;
        for (auto widget: m_vecTWidget_Occupancy)
        {
            widget->Clear();
            widget->GetCanvas()->cd();
            m_vecHistOccupancy.at(i)->Draw();
            widget->Refresh();
            i++;
        }
    }

    void DataTestTab::receiveHists(const std::map<Cbc *, TH1F *> graph, std::string option)
    {
        qDebug() << "recieved graphs";

        //m_mapThreshold = graph; //copy of shared pointer

        /*for (auto kv : graph)
        {
            m_vecTWidget_Threshold.at(0)->Clear();
             m_vecTWidget_Threshold.at(0)->GetCanvas()->cd();
            graph.at(kv)->Draw(option.c_str());
            //m_vecHistThreshold.at(0)->GetCanvas()->cd();
            //m_vecHistThreshold.at(0)->Draw(option.c_str());
            m_vecTWidget_Threshold.at(0)->Refresh();
        }*/
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

    void DataTestTab::getIsHoleModeChecked()
    {
        emit sendIsHoleModeChecked(ui->checkHoleMode->isChecked());
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
