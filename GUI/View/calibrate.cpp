#include "calibrate.h"
#include "ui_calibrate.h"

Calibrate::Calibrate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Calibrate)
{
    ui->setupUi(this);
}

Calibrate::~Calibrate()
{
    delete ui;
}
