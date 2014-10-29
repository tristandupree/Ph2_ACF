#pragma once

#include <QWidget>
#include "TH1D.h"
#include "TCanvas.h"
#include "lib/CustomTQtWidget.h"
#include "lib/TQtWidget.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <memory>
#include <QTabWidget>

namespace Ui {
    class DataTestTab;
}

namespace GUI{

    class DataTestTab : public QWidget
    {
        Q_OBJECT

    public:
        explicit DataTestTab(QWidget *parent);
        TCanvas* passTCanvas(int i);

    ~DataTestTab();
    signals:
        void notifyAddGraph();
        void sendVcthValue(int cVcth);
        void sendEventsNumber(int cEvents);
        void sendTCanvas (const std::vector<TCanvas*> canvas);

    public slots:
        void getVcthDialValue();
        void getEventsDial();
        void onDataTestStart();
        void onDataTestFinish();
        void getTCanvas();
        void refreshTCanvas();
        void setupCanvas(const bool cbc2);
        void drawGraph(const std::vector<std::shared_ptr<TH1D>> hists);


    private slots:
        void on_btnStart_clicked();

        void on_dialVcth_sliderMoved(int position);

        void on_txtVcth_editingFinished();

        void on_dialEvents_sliderMoved(int position);

        void on_txtEvents_editingFinished();

    private:

        Ui::DataTestTab *ui;

        std::vector<TCanvas*> m_vecTCanvas;
        std::vector<std::shared_ptr<TH1D>> m_vecHist;

        std::vector<TQtWidget*> m_vectorCanvas;
        std::vector<QGroupBox*> m_vectorGroupBox;
        std::vector<QHBoxLayout*> m_vectorLayout;

        int m_Vcth;
        int m_Events;

        QTabWidget *createCbcTab();
        QTabWidget *m_tabMainCbc;
    };

}
