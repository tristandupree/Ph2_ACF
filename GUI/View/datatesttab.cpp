#include "lib/CustomTQtWidget.h"
#include "datatesttab.h"
#include "ui_datatesttab.h"
//#include "lib/CustomTQtWidget.h"
#include "lib/TQtWidget.h"
#include <QDebug>
#include <memory>make
#include <TCanvas.h>
#include "TPad.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TRint.h"
#include "TROOT.h"
#include <QGroupBox>
#include <QHBoxLayout>

#include "utils.h"

namespace GUI {

    DataTestTab::DataTestTab(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::DataTestTab)

    {
        ui->setupUi(this);
        setupCanvas(2);
    }

    DataTestTab::~DataTestTab()
    {
        //qDebug() << "Destructing " << this;
        //m_vectorCanvas.clear();

        delete ui;
    }

    void DataTestTab::setupCanvas(int cNCbc)
    {
        for (int i=0; i<cNCbc; i++)
        {

            m_vectorCanvas.push_back(new TQtWidget(this));

            //m_vectorCanvas[i]->GetCanvas()->SetFillColor(i);
            QHBoxLayout *loH = new QHBoxLayout;

            loH->addWidget(m_vectorCanvas[i]);
            m_vectorLayout.push_back(loH);

            QGroupBox *gbCanvas = new QGroupBox(this);
            QString title = QString("CBC %1").arg(i);
            gbCanvas->setTitle(title);
            gbCanvas->setLayout(m_vectorLayout[i]);
            m_vectorGroupBox.push_back(gbCanvas);

            ui->loCbcBox->addWidget(m_vectorGroupBox[i]);
        }
    }

    void DataTestTab::drawGraph(const std::vector<TH1D*> hists)
    {
        for (size_t i=0; i<m_vectorCanvas.size(); i++)
        {
            TH1D *h = hists.at(i);
            m_vectorCanvas.at(i)->cd();
            h->Draw();
            m_vectorCanvas.at(i)->Refresh();
        }
    }

    void DataTestTab::drawTest()
    {
        static Int_t HistoID = 1;
        qDebug() << "in Testing env ";
        std::vector<TH1D*> graphs;
        std::vector<TCanvas*> vCanvas;

        TString name("h1_");
        Bool_t build = false;
        for (int i = 0; i <m_vectorCanvas.size() ; i++)
        {
            std::unique_ptr<TCanvas> uCanvas(new TCanvas(build));
            TCanvas *cCanvas = new TCanvas(build);
            name += HistoID++;

            vCanvas.push_back(cCanvas);
            vCanvas.at(i)->cd();


            TH1D *h1 = new TH1D(name.Data(),name.Data(),10,0, 10);
            graphs.push_back(h1);
            graphs.at(i)->Fill(i);

            //graphs.at(i)->Draw();
            graphs.at(i)->DrawCopy();

            m_vectorCanvas.at(i)->GetCanvas()->SetFillColor(i+5);
            m_vectorCanvas.at(i)->cd();

            qDebug() << i;

            m_vectorCanvas.at(i)->GetCanvas()->SetCanvas(vCanvas.at(i));
            m_vectorCanvas.at(i)->Refresh();
        }
    }


    void DataTestTab::on_btnStart_clicked()
    {
        //drawTest();

        emit notifyAddGraph();
    }
}


