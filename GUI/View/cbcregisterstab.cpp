#include "cbcregisterstab.h"
#include "ui_cbcregisterstab.h"

#include <QVector>
#include <QMap>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QStandardItem>
#include <QTableView>
#include <QStandardItem>
#include <QList>
#include <QScrollArea>
#include <QGroupBox>

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
            m_widgetMap.clear();
        }

        for (int i=0; i<cNCbc; i++)
        {
            QString title = QString("CBC %1").arg(i);
            m_tabCbc->addTab(createCbcTab(), title);
        }
    }


    void CbcRegistersTab::createCbcRegisterValue(const int cbc, const std::map<std::string, CbcRegItem> mapReg) //for initial creation - later should find inside map
    {
        int row = 0;
        int column = 0;

        for (auto& kv : mapReg)
        {
            QGridLayout *loCbcPage = m_loGridVec.at(cbc).at(kv.second.fPage); //access cbc->page
            QHBoxLayout *loHorz = new QHBoxLayout; //will contain label + text edit

            auto cAddress = kv.second.fAddress;
            QString cHexAddress;

            QString cRegTitle_Address = QString("%1 [0x%2]").arg(QString::fromStdString(kv.first), cHexAddress.setNum(cAddress, 16));

            QLabel *lblRegTitle = new QLabel(this);
            lblRegTitle->setText(cRegTitle_Address);
            //lblRegTitle->setLineWidth(150);

            QLineEdit *lineRegValue = new QLineEdit(this);
            lineRegValue->setText(QString::number(kv.second.fValue));
            lineRegValue->setMaximumWidth(50);

            loHorz->addWidget(lblRegTitle);
            loHorz->setAlignment(lblRegTitle, Qt::AlignLeft);
            loHorz->addWidget(lineRegValue);
            loHorz->setAlignment(lineRegValue, Qt::AlignLeft);

            //QScrollArea *scrollArea = new QScrollArea;
            //scrollArea->setLayout(loHorz);

            loCbcPage->addLayout(loHorz, row, column);
            loCbcPage->setAlignment(Qt::AlignLeft);

            QMap<QString, QMap<QLabel*, QLineEdit*>> mapWidgets;
            QMap<QLabel*, QLineEdit*> map;
            map.insert(lblRegTitle, lineRegValue);
            mapWidgets.insert(QString::fromStdString(kv.first), map );
            m_widgetMap.push_back(mapWidgets); //TODO use this to send later specific registers

            ++row;

            if (row == 20)
            {
                ++column;
                row = 0;
            }
        }
    }

    QTabWidget *CbcRegistersTab::createCbcTab()
    {
        QTabWidget *tabCbc = new QTabWidget;

        std::vector<QGridLayout*> loVec; //to add to master layout

        for(int i=0; i<2; i++) //number of pages
        {
            QTabWidget *tabPage = new QTabWidget;

            QGridLayout *loGrid = new QGridLayout;
            tabPage->setLayout(loGrid);

            QString title = QString("Page %1").arg(i);
            tabCbc->addTab(tabPage, title);

            loVec.push_back(loGrid);
        }
        m_loGridVec.push_back(loVec);

        return tabCbc;
    }

    void CbcRegistersTab::on_pushButton_clicked()
    {
        emit refreshCbcRegisters();
    }

}
