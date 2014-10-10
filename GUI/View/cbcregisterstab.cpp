#include "cbcregisterstab.h"
#include "ui_cbcregisterstab.h"

#include <QVector>
#include <QMap>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>

#include "../HWDescription/Cbc.h"
#include "../HWDescription/Module.h"
#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../HWInterface/Utilities.h"
#include "../Model/systemcontroller.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;
//using namespace Ph2_System;

namespace GUI {

    CbcRegistersTab::CbcRegistersTab(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::CbcRegistersTab)
    {
        ui->setupUi(this);
        //createCbcRegGrid(2);
    }

    CbcRegistersTab::~CbcRegistersTab()
    {
        qDebug() << "Destructing " << this;
        delete ui;
    }

    void CbcRegistersTab::createCbcRegGrid(int cNCbcs)//Cbc(regvalues())
    {
        QMap<QString, QWidget*> mapTabs;

        for(int i=0; i<cNCbcs; i++)
        {
            QString title = QString("CBC%1").arg(i);
            mapTabs.insert(title, new QWidget());

        }

        for (auto& kv : mapTabs.keys())
        {
            QGridLayout *loGrid = new QGridLayout(this);
            for(int j=0; j<10; j++)
            {

                QLabel *lbl = new QLabel(QString("Hello"));
                loGrid->addWidget(lbl, j, j);
            }
            mapTabs.value(kv)->setLayout(loGrid);
            ui->cbcTabs->addTab(mapTabs.value(kv), kv);
        }

    }
}
