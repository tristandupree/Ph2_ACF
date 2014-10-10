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

}
