#include "lib/CustomTQtWidget.h"
#include "datatesttab.h"
#include "ui_datatesttab.h"
#include "lib/TQtWidget.h"
#include <QDebug>
#include <memory>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QTabWidget>

#include "utils.h"

namespace GUI {

    DataTestTab::DataTestTab(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::DataTestTab),
        m_Vcth(0),
        m_Events(0),
        m_tabMainCbc(new QTabWidget)
    {
        ui->setupUi(this);
        ui->loCbcBox->addWidget(m_tabMainCbc);
        setupCanvas(true);
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
        if(m_tabMainCbc->count() != m_vectorCanvas.size())
        {
            qDebug() << "ERROR! Canvas size does not match tab size";
            qDebug() << "Cbc Tabs: " << m_tabMainCbc->count();
            qDebug() << "Canvas size : " << m_vectorCanvas.size();
        }

        if(m_tabMainCbc->count() > 0)
        {
            m_tabMainCbc->clear();
            m_vectorCanvas.clear();
        }

        for (int i=0; i<cNCbc; i++)
        {
            m_vectorCanvas.push_back(new TQtWidget(this));
            QString title = QString("CBC %1").arg(i);
            m_tabMainCbc->addTab(createCbcTab(), title);
        }
    }

    QTabWidget *DataTestTab::createCbcTab()
    {
        QTabWidget *tab = new QTabWidget;
        QHBoxLayout *loH = new QHBoxLayout;

        loH->addWidget(m_vectorCanvas.at(m_vectorCanvas.size()-1));
        m_vectorLayout.push_back(loH);

        tab->setLayout(loH);
        return tab;
    }

    void DataTestTab::drawGraph(const std::vector<std::shared_ptr<TH1D> > hists)
        {
            for (size_t i=0; i<m_vectorCanvas.size(); i++)
            {
                m_vecHist = hists;
                TQtWidget *u = new TQtWidget(this); //No idea why this helps flush
                //m_vecHist = hists.a
                m_vectorCanvas.at(i)->cd();
                m_vecHist.at(i)->Draw();
                m_vectorCanvas.at(i)->Refresh();
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

    void DataTestTab::onDataTestStart()
    {
        ui->btnStart->setEnabled(false);
    }

    void DataTestTab::onDataTestFinish()
    {
        ui->btnStart->setEnabled(true);
    }

    void DataTestTab::getTCanvas()
    {
        for (auto& canvas : m_vectorCanvas)
        {
            m_vecTCanvas.push_back(canvas->GetCanvas());
        }

        emit sendTCanvas(m_vecTCanvas);
    }

    void DataTestTab::refreshTCanvas()
    {
        for (auto& canvas : m_vectorCanvas)
        {
            TQtWidget *u = new TQtWidget(this); //No idea why this helps flush
            qDebug() << " >>> Refreshed Canvas" ;
            canvas->Refresh();
        }
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
