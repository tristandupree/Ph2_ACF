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

        clearTabs();

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

        //createCbcRegisterValue(0,testing);
        //createCbcRegisterValue(1,testing);

    }


    void CbcRegistersTab::createCbcRegisterValue(const int cbc, const std::map<std::string, CbcRegItem> mapReg) //for initial creation - later should find inside map
    {
        int row = 0;
        int column = 0;
        int cLineSize = 0;

        m_widgetMap.clear();

        for (auto& kv : mapReg)
        {

            QGridLayout *loCbcPage = m_loGridVec.at(cbc).at(kv.second.fPage); //access cbc->page

            QHBoxLayout *loHorz = new QHBoxLayout; //will contain label + text edit

            auto cAddress = kv.second.fAddress;

            QString cHexAddress;
            QLabel *lblRegTitle = new QLabel(this);
            QLabel *lblRegAddress = new QLabel(this);

            lblRegTitle->setText(QString::fromStdString(kv.first));
            lblRegAddress->setText(QString("[0x%1]").arg(cHexAddress.setNum(cAddress,16)));

            if (cLineSize < lblRegTitle->width())
            {
                cLineSize = lblRegTitle->width(); //find min width needed
            }

            QLineEdit *lineRegValue = new QLineEdit(this);
            lineRegValue->setText(QString::number(kv.second.fValue));
            lineRegValue->setMaximumWidth(30);

            loHorz->addWidget(lblRegTitle);
            loHorz->addWidget(lblRegAddress);
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

        for(auto& cCbc : m_widgetMap) //vector of CBCs
        {
            for(auto& regNames : cCbc.keys()) //registerName keys
            {
                for(auto& widget : cCbc.value(regNames).keys())
                {
                    widget->setMinimumWidth(cLineSize + 50);
                    cCbc.value(regNames).value(widget)->setMinimumWidth(20);
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
            QWidget *client = new QWidget; //client widget for scroll area
            QScrollArea *scrollArea = new QScrollArea;
            scrollArea->setWidgetResizable(true);
            scrollArea->setWidget(client);
            QGridLayout *loGrid = new QGridLayout;
            client->setLayout(loGrid);

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

    void CbcRegistersTab::createCbcRegItems()
    {
        int nCbc = 0;

        for(auto& cCbc : m_widgetMap) //vector of CBCs
        {
            std::vector<std::pair<std::string, std::uint8_t>> vecRegValues;
            for(auto& regNames : cCbc.keys())//registerName keys
            {
                for(auto& widget : cCbc.value(regNames).keys())
                {
                    std::string regTitle = (widget->text()).toStdString();
                    std::string regValueTemp= (cCbc.value(regNames).value(widget)->text()).toStdString();

                    std::vector<uint8_t> stupidConversion(regValueTemp.begin(), regValueTemp.end());
                    std::uint8_t regValue = *&stupidConversion[0];

                    vecRegValues.push_back(std::make_pair(regTitle, regValue));
                }
            }
            emit sendCbcRegisters(nCbc, vecRegValues);
            ++nCbc;
        }
    }

    void CbcRegistersTab::clearTabs()
    {
        m_tabCbc->clear();
        m_loGridVec.clear();
        m_widgetMap.clear();
    }

    void CbcRegistersTab::on_btnRefresh_clicked()
    {
        emit refreshCbcRegisters();
    }
    void CbcRegistersTab::on_btnUpdate_clicked()
    {
        qDebug() << "Pressed..";
        createCbcRegItems();
        emit refreshCbcRegisters();
        qDebug() << "Pressed";
    }

}

