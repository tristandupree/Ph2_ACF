#ifndef DATATESTTAB_H
#define DATATESTTAB_H

#include <QWidget>
#include "TH1D.h"
#include "TCanvas.h"
#include "lib/CustomTQtWidget.h"
#include "lib/TQtWidget.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <memory>

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
        void sendTCanvas (std::vector<TCanvas*> canvas);

    public slots:
        void drawGraph(const std::vector<std::shared_ptr<TH1D>> hists);
        void getVcthDialValue();
        void getEventsDial();
        void onDataTestStart();
        void onDataTestFinish();
        void getTCanvas();
        void refreshTCanvas();


    private slots:
        void on_btnStart_clicked();

        void on_dialVcth_sliderMoved(int position);

        void on_txtVcth_editingFinished();

        void on_dialEvents_sliderMoved(int position);

        void on_txtEvents_editingFinished();

    private:

        Ui::DataTestTab *ui;

        std::vector<std::shared_ptr<TH1D>> m_vecHist;

        std::vector<TCanvas*> m_vecTCanvas;

        std::vector<TQtWidget*> m_vectorCanvas;
        std::vector<QGroupBox*> m_vectorGroupBox;
        std::vector<QHBoxLayout*> m_vectorLayout;

        int m_Vcth;
        int m_Events;

        void setupCanvas(int cNCbc);
    };

}
#endif // DATATESTTAB_H
