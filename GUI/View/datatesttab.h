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
#include "../HWDescription/Cbc.h"

namespace Ui {
    class DataTestTab;
}

using namespace Ph2_HwDescription;

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
        void sendIsHoleModeChecked(const bool);

    public slots:
        void getVcthDialValue();
        void getEventsDial();
        void getIsRegTestChecked();
        void getIsScanChecked();
        void getIsHoleModeChecked();
        void onDataTestStart();
        void onDataTestFinish();
        void setupCanvas(const bool cbc2);

        void receiveOccupancyHists(const std::vector<std::shared_ptr<TH1F>> hists);
        void receiveSCurve(const std::map<std::shared_ptr<Cbc>, std::shared_ptr<TH1F>> graph, std::string option);
        void receiveSCurve(const std::map<std::shared_ptr<Cbc>, std::shared_ptr<TF1>> graph, std::string option);
        void receiveRefreshHists();

    private slots:
        void on_btnStart_clicked();

        void on_dialVcth_sliderMoved(int position);

        void on_txtVcth_editingFinished();

        void on_dialEvents_sliderMoved(int position);

        void on_txtEvents_editingFinished();

    private:

        Ui::DataTestTab *ui;
        std::vector<std::shared_ptr<TH1F>> m_vecHistOccupancy;

        std::vector<TQtWidget*> m_vecTWidget_Occupancy;
        std::vector<TQtWidget*> m_vecTWidget_Threshold;
        std::vector<QGroupBox*> m_vectorGroupBox;
        std::vector<QHBoxLayout*> m_vectorLayout;

        std::map<std::shared_ptr<Cbc>, std::shared_ptr<TH1F>> m_mapSCurve;
        std::map<std::shared_ptr<Cbc>, std::shared_ptr<TF1>> m_mapFit;


        int m_Vcth;
        int m_Events;

        QTabWidget *createOccupancyTab();
        QTabWidget *m_tabMainCbc;
        QTabWidget *createThresholdTab();
    };

}
