#include "lib/CustomTQtWidget.h"
#include "datatesttab.h"
#include "ui_datatesttab.h"
//#include "lib/CustomTQtWidget.h"
#include "lib/TQtWidget.h"
#include <QDebug>
#include <memory>make
#include <TCanvas.h>
#include "TPad.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TRint.h"
#include "TROOT.h"
namespace GUI {

    DataTestTab::DataTestTab(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::DataTestTab)
      //h1(new TH1D("Histo","HistoName",10,0, 10))

    {
        ui->setupUi(this);
        //drawTest();
        //setupCanvas("CBC1", 2);
        //ui->gbTestTab->setEnabled(false);
    }

    DataTestTab::~DataTestTab()
    {
        //qDebug() << "Destructing " << this;
        delete ui;
    }

    void DataTestTab::setupCanvas(QString graphTitle, int nCbc)
    {
        //ui->graphDataTest->cd();
        //h1->Fill(1);
        //ui->graphDataTest->Draw(h1);


        //ui->graphDataTest->GetCanvas()->SetFillColor(5);
        //ui->graphDataTest->GetCanvas()->Divide(2, 1); //col,row

        //TH1D* h1 = new TH1D("Histo","HistoName",10,0, 10);
        //h1->Fill(1);
        //h1->Draw();
        //ui->graphDataTest->Draw(h1);

        //TCanvas* cCanvas = new TCanvas( "Data Acq", "Different hits counters", 1000, 800 );
        //cCanvas->Divide( 2, 1 );
        //ui->graphDataTest->

    }

    //void DataTestTab::drawGraph(const QVector<double> &x, const QVector<double> &y)
    void DataTestTab::drawGraph(const std::vector<TH1F *> &value)
    {
        //TCanvas* cCanvas = new TCanvas( "Data Acq", "Different hits counters", 1000, 800 );
        //cCanvas->Divide( 2, 1 );
        ui->graphDataTest->GetCanvas()->cd();
        ui->graphDataTest->GetCanvas()->Clear();
        //ui->graphDataTest->SetCanvas();
        //ui->graphDataTest->GetCanvas()->Update();
        //gPad->SetFillColor(3);
        ui->graphDataTest->Refresh();
        ui->graphDataTest->Draw(value[0]);
        ui->graphDataTest->GetCanvas()->Print("output.pdf");
        ui->graphDataTest->Refresh();

        //ui->graphDataTest->

        //        /cCanvas->Print( ( ( boost::format( "output/Histogram_Vcth_%d.pdf" ) % ( cVCth ) ).str() ).c_str() );

        for ( uint8_t cNCbc = 0; cNCbc < value.size(); cNCbc++)
        {
            qDebug() << cNCbc << " " << value[cNCbc];
            // delete value[cNCbc];
        }
    }

    void DataTestTab::drawTest()
    {
        gROOT->SetBatch(kTRUE);
        TCanvas* cCanvas = new TCanvas( "Data Acq", "Different hits counters", 1000, 800 );
        cCanvas->Divide( 2, 1 );
        ui->graphDataTest->GetCanvas()->SetCanvas(cCanvas);

        for (int i = 1; i <10 ; i++)
        {
            //std::shared_ptr<TH1D> h1(new TH1D("Histo","HistoName",10,0, 10));

            ui->graphDataTest->Clear();
            ui->graphDataTest->Refresh();

            TH1D* h1 = new TH1D("Histo","HistoName",10,0, 10);
            h1->Fill(i);
            ui->graphDataTest->Draw(h1);
        }
        //delete h1;
    }

    void DataTestTab::on_btnStart_clicked()
    {
        drawTest();

        //emit notifyAddGraph();

    }
}


