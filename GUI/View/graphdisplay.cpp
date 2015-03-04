#include "graphdisplay.h"
#include "ui_graphdisplay.h"

GraphDisplay::GraphDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphDisplay)
{
    ui->setupUi(this);
}

GraphDisplay::~GraphDisplay()
{
    delete ui;
}
