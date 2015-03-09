#include "cbcregisterstab.h"
#include "ui_cbcregisterstab.h"

#include <QFont>
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

        /*std::map<std::string, CbcRegItem> testing;
        for(int i=0; i<50; i++)
        {
            std::string tit = "Test";
            tit.append(std::to_string(i));

            testing.insert( std::pair<std::string, CbcRegItem>(tit,test ) );
        }

        createCbcRegisterValue(0,testing);
        createCbcRegisterValue(1,testing);*/

    }


    void CbcRegistersTab::createCbcRegisterValue(const int cbc, const std::map<std::string, CbcRegItem> mapReg) //for initial creation - later should find inside map
    {
        int row = 0;
        int column = 0;
        int cSizeTitle = 0;
        int cSizeAddr = 0; //TODO size by page number
        QMap<QString, QLineEdit*> cWidgetMap;

        for (auto& kv : mapReg)
        {
            QFont font("Sans Serif", 8);
            QHBoxLayout *loHorz = new QHBoxLayout; //Lo to add to grid
            QSpacerItem *spacer = new QSpacerItem(50,50); //TODO play with this

            QLabel *lblRegTitle = new QLabel(this);
            QLabel *lblRegAddress = new QLabel(this);

            lblRegTitle->setText(QString::fromStdString(kv.first));
            lblRegTitle->setFont(font);

            auto cAddress = kv.second.fAddress;
            if (cAddress < 16) lblRegAddress->setText(QString("[0x0%1]").arg(QString::number(cAddress,16))); //appends padding 0
            else lblRegAddress->setText(QString("[0x%1]").arg(QString::number(cAddress,16)));
            lblRegAddress->setFont(font);

            QLineEdit *lineRegValue = new QLineEdit(this);
            lineRegValue->setFixedWidth(30);
            lineRegValue->setText(QString::number(kv.second.fValue));
            lineRegValue->setFont(font);

            loHorz->addWidget(lblRegTitle);
            loHorz->setAlignment(lblRegTitle, Qt::AlignLeft);
            loHorz->addWidget(lblRegAddress);
            loHorz->setAlignment(lblRegAddress, Qt::AlignLeft);
            loHorz->addWidget(lineRegValue);
            loHorz->setAlignment(lineRegValue, Qt::AlignLeft);
            loHorz->addSpacerItem(spacer);
            loHorz->addStretch(5);

            m_loGridVec.at(cbc).at(kv.second.fPage)->addLayout(loHorz, row, column);

            cWidgetMap.insert(QString::fromStdString(kv.first),lineRegValue);

            if (cSizeTitle < lblRegTitle->width()) cSizeTitle = lblRegTitle->width(); //find min width needed
            if (cSizeAddr < lblRegAddress->width()) cSizeAddr = lblRegAddress->width();

            ++row;

            if (row == 20)
            {
                ++column;
                row = 0;
            }
        }
        m_widgetMap.push_back(cWidgetMap);
    }

    void CbcRegistersTab::updateCbcRegisterValues(const int cbc, const std::map<std::string, CbcRegItem> mapReg)
    {
        /*for(auto &kv : mapReg)
        {
            auto cValue = kv.second.fValue;
            //qDebug() << QString::fromStdString(kv.first) << "  " << cValue;

            //std::cout << m_widgetMap.at(cbc).value(kv.first);

            //qDebug() << m_widgetMap.at(cbc).value(kv.first);
            //m_widgetMap.at(cbc).value(kv)->setText(QString::number(cValue));
        }*/
    }

    QTabWidget *CbcRegistersTab::createCbcTab()
    {
        QTabWidget *tabCbc =  new QTabWidget(this);

        QVector<QGridLayout*> loVec; //to add to master layout

        for(int i=0; i<2; i++) //number of REGISTER pages
        {
            QWidget *client = new QWidget; //client widget for scroll area
            QScrollArea *scrollArea = new QScrollArea;
            scrollArea->setWidgetResizable(true);
            scrollArea->setWidget(client); //add scroll area to client
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

        for(auto& cCbc : m_widgetMap)
        {
            std::vector<std::pair<std::string, std::uint8_t>> vecRegValues;
            for (auto& regName : cCbc.keys())
            {
                std::string regTitle = regName.toStdString();

                std::string regValueTemp = (cCbc.value(regName)->text()).toStdString();
                std::vector<uint8_t> stupidConversion(regValueTemp.begin(), regValueTemp.end());
                std::uint8_t regValue = *&stupidConversion[0];

                vecRegValues.push_back(std::make_pair(regTitle, regValue));
                std::cout << regTitle << "  " << regValue<< std::endl;
            }
            qDebug() << "init";
            emit writeCbcRegisters(nCbc, vecRegValues);
            qDebug() << "done";
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

    void CbcRegistersTab::on_btnWrite_clicked()
    {
        createCbcRegItems();
        //emit updateCbcRegisters();
    }


}
