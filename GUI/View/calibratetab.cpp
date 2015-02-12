#pragma once

#include "calibratetab.h"
#include "ui_calibratetab.h"

namespace GUI{

    CalibrateTab::CalibrateTab(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::CalibrateTab)
    {
        ui->setupUi(this);
    }

    CalibrateTab::~CalibrateTab()
    {
        delete ui;
    }
}
