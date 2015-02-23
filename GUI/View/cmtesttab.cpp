#pragma once

#include "cmtesttab.h"
#include "ui_cmtesttab.h"

namespace GUI {

    CmTestTab::CmTestTab(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::CmTestTab)
    {
        ui->setupUi(this);
    }

    CmTestTab::~CmTestTab()
    {
        delete ui;
    }
}
