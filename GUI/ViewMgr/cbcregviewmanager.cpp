#include "cbcregviewmanager.h"

//#include <QStandardItem>
#include <QStandardItemModel>

#include <QDebug>

namespace GUI
{
    CbcRegViewManager::CbcRegViewManager(QObject *parent) :
        QObject(parent)
    {
    }

    CbcRegViewManager::~CbcRegViewManager()
    {
        qDebug() << "Destructing " << this;
    }

    void CbcRegViewManager::createGrid()
    {
        QString text = "Hello World";

        QStandardItemModel *model = new QStandardItemModel(2,3,this); //2 Rows and 3 Columns

        model->setHorizontalHeaderItem(0, new QStandardItem(QString("Column1 Header")));
        model->setHorizontalHeaderItem(1, new QStandardItem(QString("Column2 Header")));
        model->setHorizontalHeaderItem(2, new QStandardItem(QString("Column3 Header")));


        QStandardItem *firstRow = new QStandardItem(QString("ColumnValue"));

        model->setItem(0,0,firstRow);



        //ui->tableView->setModel(model);


        //QStandardItem test = new QStandardItem(text);

        //model->setHorizontalHeaderItem(0, text);

        /*model->setHorizontalHeaderItem(0, new QStandardItem(QString("Column1 Header")));
        model->setHorizontalHeaderItem(1, new QStandardItem(QString("Column2 Header")));
        model->setHorizontalHeaderItem(2, new QStandardItem(QString("Column3 Header")));*/


        //QStandardItem *firstRow = new QStandardItem(QString("ColumnValue"));

        //model->setItem(0,0,firstRow);
    }
}
