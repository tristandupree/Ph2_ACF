#include "setuptab.h"
#include "ui_setuptab.h"

#include <QDebug>
#include <QVariantMap>

#include <QListView>

#include <QStandardItemModel>

namespace GUI{

    const int ROWS = 2;
    const int COLUMNS = 3;

    SetupTab::SetupTab(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::SetupTab)
    {
        ui->setupUi(this);
    }

    SetupTab::~SetupTab()
    {
        qDebug() << "Destructing " << this;
        delete ui;
    }

    void SetupTab::setHwTreeView(QStandardItemModel* model)
    {
        ui->treeTest->setModel(model);

        ui->btnInit->setEnabled(true);
        ui->btnConfig->setEnabled(false);
    }

    void SetupTab::onStatusUpdate(const QString &statusMsg)
    {
        ui->editMsgBox->append(statusMsg);
    }

    bool SetupTab::isCbc2Checked ()
    {
        return ui->radio2CBC->isChecked();
    }

    void SetupTab::on_radio2CBC_clicked()
    {
        ui->radio2CBC->toggle();
    }

    void SetupTab::on_radio8CBC_clicked()
    {
        ui->radio8CBC->toggle();
    }

    void SetupTab::on_btnConfig_clicked()
    {
        emit onBtnCfgClicked();
        ui->btnConfig->setEnabled(false);
        ui->btnInit->setEnabled(false);
    }

    void SetupTab::on_btnLoad_clicked()
    {
        emit onBtnLoadSettingsClicked(isCbc2Checked());

    }

    void SetupTab::on_btnInit_clicked()
    {
        emit onBtnInitClicked();
        ui->btnConfig->setEnabled(true);
        ui->btnInit->setEnabled(false);
        //ui->btnInit->setDisabled(true);
    }
}




