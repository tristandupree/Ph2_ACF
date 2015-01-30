#include "tbrowsertab.h"
#include "ui_tbrowsertab.h"

#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QString>
#include <QByteArray>
#include "TBrowser.h"
#include "TServerSocket.h"

namespace GUI{

    TBrowserTab::TBrowserTab(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::TBrowserTab)
    {
        ui->setupUi(this);
    }

    TBrowserTab::~TBrowserTab()
    {
        qDebug() << "Destructing " << this;
        delete ui;
    }
   /* void TBrowserTab::on_pushButton_clicked()
    {

        /*QByteArray ba;

        if (process->waitForStarted(-1)) {
            while(process->waitForReadyRead(-1)) {
                ba += process->readAllStandardOutput();
            }
        }

        char *someData = ba.data();

        qDebug() << "Testing QProcess ..." ;
        qDebug() << someData;
        qDebug() << "done!" ;

        //TServerSocket *ss = new TServerSocket(9090, kTRUE);



        //auto *p = new TBrowser();

    }*/

    void TBrowserTab::on_Calibrate_clicked()
    {
        QProcess *process = new QProcess(this);
        process->start("./bin/calibrate");
    }

    void TBrowserTab::on_CmTest_clicked()
    {
        QProcess *process = new QProcess(this);
        process->start("./bin/cmtest");
    }

    void TBrowserTab::on_DataTest_clicked()
    {
        QProcess *process = new QProcess(this);
        process->start("./bin/datatest");
    }

    void TBrowserTab::on_HybridTest_clicked()
    {
        QProcess *process = new QProcess(this);
        process->start("./bin/hybridtest");
    }

    void TBrowserTab::on_InterfaceTest_clicked()
    {
        QProcess *process = new QProcess(this);
        process->start("./bin/interfacetest");
    }

    void TBrowserTab::on_pushButton_6_clicked()
    {
        QProcess *process = new QProcess(this);
        process->start("./bin/systemtest");
    }

    void TBrowserTab::on_TBrowser_clicked()
    {
        qDebug() << "I'm in";
        QProcess *process = new QProcess(this);
        process->start("./CreatePDF");
    }
}
