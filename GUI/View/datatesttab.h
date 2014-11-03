#pragma once

#include <QWidget>
#include "TH1F.h"
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

    ~DataTestTab();
    signals:
        void notifyAddGraph();
        void sendVcthValue(int cVcth);
        void sendEventsNumber(int cEvents);
        void sendIsRegTestChecked(const bool);
        void sendIsScanChecked(const bool);

    public slots:
        void getVcthDialValue();
        void getEventsDial();
        void getIsRegTestChecked();
        void getIsScanChecked();
        void onDataTestStart();
        void onDataTestFinish();
        void setupCanvas(const bool cbc2);
        void drawOccupancy(const std::vector<std::shared_ptr<TH1F>> hists);


    private slots:
        void on_btnStart_clicked();

        void on_dialVcth_sliderMoved(int position);

        void on_txtVcth_editingFinished();

        void on_dialEvents_sliderMoved(int position);

        void on_txtEvents_editingFinished();

    private:

        Ui::DataTestTab *ui;
        std::vector<std::shared_ptr<TH1F>> m_vecHistOccupancy;

        TCanvas* m_canvasOccupy;

        std::vector<TQtWidget*> m_vecTWidget;
        std::vector<QGroupBox*> m_vectorGroupBox;

        int m_Vcth;
        int m_Events;

        QTabWidget *createCbcTab();
        QTabWidget *m_tabMainCbc;
    };

}
