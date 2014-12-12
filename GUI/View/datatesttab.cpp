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
#include "TLine.h"

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

        QString titleOcc = QString("Occupancy");
        m_tabMainCbc->addTab(createOccupancyTab(), titleOcc);

        QString titleThresh = QString("Threshold Scan");
        m_tabMainCbc->addTab(createThresholdTab(), titleThresh);
    }

    QTabWidget *DataTestTab::createOccupancyTab()
    {
        QTabWidget *tabOcc = new QTabWidget;
        QHBoxLayout *loHOcc = new QHBoxLayout;

        for (int i=0; i<2 ;i++) //TODO support 8CBC
        {
            m_vecTWidget_Occupancy.push_back(new TQtWidget(this));
            loHOcc->addWidget(m_vecTWidget_Occupancy.at(i));
            m_vectorLayout.push_back(loHOcc); //incase I want to access later
            tabOcc->setLayout(loHOcc);
        }
        return tabOcc;
    }

    QTabWidget *DataTestTab::createThresholdTab()
    {
        QTabWidget *tabThresh = new QTabWidget;
        QHBoxLayout *loHThresh = new QHBoxLayout;

        for (int i=0; i<2 ;i++) //TODO support 8CBC
        {
            m_vecTWidget_Threshold.push_back(new TQtWidget(this));
            loHThresh->addWidget(m_vecTWidget_Threshold.at(i));
            m_vectorLayout.push_back(loHThresh); //incase I want to access later
            tabThresh->setLayout(loHThresh);
        }
        return tabThresh;
    }

    void DataTestTab::receiveOccupancyHists(const std::vector<std::shared_ptr<TH1F> > hists)
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

    void DataTestTab::receiveSCurve(const std::map<std::shared_ptr<Cbc>, std::shared_ptr<TH1F> > graph, std::string option)
    {
        m_mapSCurve = graph;
        auto cSigmas = 4; //TODO get GUI to send Sigmas here e.g getSigmas()
        int i = 0;
        for(const auto& cbcHistoPair : m_mapSCurve)
        {
            auto flush = new TQtWidget(this);
            TString title =  cbcHistoPair.second->GetTitle();
            m_vecTWidget_Threshold.at(i)->Clear();
            m_vecTWidget_Threshold.at(i)->GetCanvas()->cd();
            cbcHistoPair.second->Draw(option.c_str());
            m_vecTWidget_Threshold.at(i)->Refresh();
            i++;
            qDebug() << "Fit S Curve" << i;
        }
        qDebug() << "End fitting";

    }

    void DataTestTab::receiveSCurve(const std::map<std::shared_ptr<Cbc>, std::shared_ptr<TF1> > graph, std::string option)
    {
        m_mapFit = graph;
        auto cSigmas = 4;
        int i = 0;
        for(const auto& cbcHistoPair : m_mapFit)
        {
            qDebug() << "FIT " << i;
            m_vecTWidget_Threshold.at(i)->GetCanvas()->cd();
            double_t pedestal = cbcHistoPair.second->GetParameter( 0 );
            double_t noise = cbcHistoPair.second->GetParameter( 1 );
            uint8_t cThreshold = ceil( pedestal + cSigmas * fabs( noise ) );
            TLine* cLine = new TLine( cThreshold, 0, cThreshold, 1 );
            cLine->SetLineWidth( 3 );
            cLine->SetLineColor( 2 );
            cLine->Draw( "same" );
            cbcHistoPair.second->Draw(option.c_str());
            m_vecTWidget_Threshold.at(i)->Refresh();
            i++;
        }
    }

    void DataTestTab::receiveRefreshHists() //Not working properly
    {
        for (int i=0; i<2; i++)
        {
            m_vecTWidget_Occupancy.at(i)->Clear();
            //m_vecHistOccupancy = hists;
            m_vecTWidget_Occupancy.at(i)->GetCanvas()->cd();
            m_vecHistOccupancy.at(i)->Draw();
            m_vecTWidget_Occupancy.at(i)->Refresh();
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
