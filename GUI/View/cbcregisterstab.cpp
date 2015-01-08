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
#include <QVBoxLayout>

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

        CbcRegItem test;
        test.fAddress=0;
        test.fDefValue=5;
        test.fPage=0;
        test.fValue=6;

        std::map<std::string, CbcRegItem> testing = {{"Hey", test},{"Hey2222", test},{"Hey33", test},{"Hey4", test},{"Hey5", test},{"Hey666666666666666", test},{"Hey7", test},{"Hey88", test}};
        qDebug() << "creating test";

        createCbcRegisterValue(0,testing);
        createCbcRegisterValue(1,testing);

    }


    void CbcRegistersTab::createCbcRegisterValue(const int cbc, const std::map<std::string, CbcRegItem> mapReg) //for initial creation - later should find inside map
    {
        int row = 0;
        int column = 0;
        int cLineSize = 0;

        for (auto& kv : mapReg)
        {

            QGridLayout *loCbcPage = m_loGridVec.at(cbc).at(kv.second.fPage); //access cbc->page

            QHBoxLayout *loHorz = new QHBoxLayout; //will contain label + text edit

            auto cAddress = kv.second.fAddress;
            QString cHexAddress;

            QString cRegTitle_Address = QString("%1 [0x%2]").arg(QString::fromStdString(kv.first), cHexAddress.setNum(cAddress, 16));

            QLabel *lblRegTitle = new QLabel(this);
            lblRegTitle->setText(cRegTitle_Address);

            if (cLineSize < lblRegTitle->width())
            {
                cLineSize = lblRegTitle->width();
            }

            QLineEdit *lineRegValue = new QLineEdit(this);
            lineRegValue->setText(QString::number(kv.second.fValue));
            lineRegValue->setMaximumWidth(30);

            loHorz->addWidget(lblRegTitle);
            loHorz->setAlignment(lblRegTitle, Qt::AlignLeft);
            loHorz->addWidget(lineRegValue);
            loHorz->setAlignment(lineRegValue, Qt::AlignLeft);

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

        for(auto& cbc : m_widgetMap) //vector of CBCs
        {
            for(auto& regNames : cbc.keys())//registerName keys
            {
                for(auto& widget : cbc.value(regNames).keys())
                {
                    //cbc.value(regNames).value(widget)->setMaximumWidth(cLineSize + 50);
                    widget->setMinimumWidth(cLineSize + 50);
                    qDebug() << widget;
                }
            }
        }
    }

    QTabWidget *CbcRegistersTab::createCbcTab()
    {
        QTabWidget *tabCbc = new QTabWidget(this);

        std::vector<QGridLayout*> loVec; //to add to master layout

        for(int i=0; i<2; i++) //number of pages
        {
            QWidget *client = new QWidget;
            QScrollArea *scrollArea = new QScrollArea;
            scrollArea->setWidgetResizable(true);
            scrollArea->setWidget(client);
            QGridLayout *loGrid = new QGridLayout;
            client->setLayout(loGrid);

            QTabWidget *tabPage = new QTabWidget;
            QWidget *pageWidget = new QWidget;
            pageWidget->setLayout(new QVBoxLayout);
            pageWidget->layout()->addWidget(scrollArea);


            QString title = QString("Page %1").arg(i);
            tabCbc->addTab(pageWidget, title);

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
