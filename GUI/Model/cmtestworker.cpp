#pragma once
#include "cmtestworker.h"
#include <QThread>
#include <QDebug>

namespace GUI{
    CmTestWorker::CmTestWorker(QObject *parent) :
        QObject(parent)
    {

    }

    CmTestWorker::~CmTestWorker()
    {
        qDebug() << "Destructing CmTest Worker " << this;
    }
}

