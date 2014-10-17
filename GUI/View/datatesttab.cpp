#include "lib/CustomTQtWidget.h"
#include "datatesttab.h"
#include "ui_datatesttab.h"
#include "lib/TQtWidget.h"
#include <QDebug>
#include <memory>
#include <QGroupBox>
#include <QHBoxLayout>

#include "utils.h"

namespace GUI {

    DataTestTab::DataTestTab(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::DataTestTab),
        m_Vcth(0),
        m_Events(0)
    {
        ui->setupUi(this);
        setupCanvas(2);
    }

    DataTestTab::~DataTestTab()
    {
        qDebug() << "Destructing " << this;
        delete ui;
    }

    void DataTestTab::setupCanvas(int cNCbc)
    {
        for (int i=0; i<cNCbc; i++)
        {
            //m_vectorCanvas.push_back(new CustomTQtWidget(this));
            m_vectorCanvas.push_back(new TQtWidget(this));
            QHBoxLayout *loH = new QHBoxLayout;

            loH->addWidget(m_vectorCanvas[i]);
            m_vectorLayout.push_back(loH);

            QGroupBox *gbCanvas = new QGroupBox(this);
            QString title = QString("CBC %1").arg(i);
            gbCanvas->setTitle(title);
            gbCanvas->setLayout(m_vectorLayout[i]);
            m_vectorGroupBox.push_back(gbCanvas);

            ui->loCbcBox->addWidget(m_vectorGroupBox[i]);
        }
    }

    void DataTestTab::drawGraph(const std::vector<std::shared_ptr<TH1D> > hists)
    {
        for (size_t i=0; i<m_vectorCanvas.size(); i++)
        {
            m_vecHist = hists;
            TQtWidget *u = new TQtWidget(this); //No idea why this helps flush
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
        qDebug() << "getTCanvas()";

        for (int i; i < m_vectorCanvas.size(); i++)
        {
            m_vecTCanvas.push_back(m_vectorCanvas.at(i)->GetCanvas());
        }
        emit sendTCanvas(m_vecTCanvas);
    }

    void DataTestTab::refreshTCanvas()
    {
        qDebug()<< "RefreshTCanvas()";
        for (int i; i < m_vectorCanvas.size(); i++)
        {
            m_vectorCanvas.at(i)->Refresh();
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
