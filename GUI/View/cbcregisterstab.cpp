#include "cbcregisterstab.h"
#include "ui_cbcregisterstab.h"

#include <QVector>
#include <QMap>
#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>

namespace GUI {

    CbcRegistersTab::CbcRegistersTab(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::CbcRegistersTab),
        m_tabCbc(new QTabWidget)
    {
        ui->setupUi(this);
        ui->loCbcs->addWidget(m_tabCbc);
        setupCbcRegGrid(true);
    }

    CbcRegistersTab::~CbcRegistersTab()
    {
        qDebug() << "Destructing " << this;
        delete ui;
    }

    void CbcRegistersTab::setupCbcRegGrid(const bool cbc2)
    {
        int cNCbc;
        if(cbc2) cNCbc = 2;
        else cNCbc = 8;

        if(m_tabCbc->count() > 0)
        {
            m_tabCbc->clear();
            m_loGridVec.clear();
        }

        for (int i=0; i<cNCbc; i++)
        {
            QString title = QString("CBC %1").arg(i);
            m_tabCbc->addTab(createCbcTab(), title);
        }
    }


    void CbcRegistersTab::createCbcRegisterValue(const int cbc, const std::map<std::string, CbcRegItem> mapReg)
    {
        qDebug() << "I'm in";
        for (auto& kv : mapReg)
        {
            qDebug() << QString::fromStdString(kv.first);
            qDebug() << QString::number(int(kv.second.fAddress));

            //QGridLayout *tabCbc = m_loGridVec.at(cbc).at(kv.second.fPage); //access cbc->page
            QGridLayout *tabCbc = m_loGridVec.at(0).at(0); //access cbc->page
            QString RegTitle_Address = QString("%1    %2").arg(QString::fromStdString(kv.first), QString::number(kv.second.fAddress));
            QString value = QString::number(kv.second.fValue);

            QLabel *lblRegTitle = new QLabel(this);
            lblRegTitle->setText(RegTitle_Address);

            tabCbc->addWidget(lblRegTitle);

        }
        //QLabel lblRegNameAndValue = new QLabel(this);
        //lblRegNameAndValue.setText(title);
    }

    QTabWidget *CbcRegistersTab::createCbcTab()
    {
        std::vector<QGridLayout*> loVec; //to add to master layout

        QTabWidget *tabCbc = new QTabWidget;

        for(int i=0; i<2; i++) //number of pages
        {
            QTabWidget *tabPage = new QTabWidget;
            QGridLayout *loGrid = new QGridLayout;

            QString title = QString("Page %1").arg(i);
            tabPage->setLayout(loGrid);

            tabCbc->addTab(tabPage, title);

            loVec.push_back(loGrid);
        }

        m_loGridVec.push_back(loVec);

        return tabCbc;
    }


}
