#pragma once

#include <QMainWindow>

namespace Ui {
    class MainView;
}

namespace GUI{

    class SetupTab;
    class CbcRegistersTab;
    class HybridTestTab;
    class CalibrateTab;
    class TBrowserTab;

    class MainView : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainView(QWidget *parent,
                          GUI::SetupTab &setup,
                          GUI::CbcRegistersTab &reg,
                          GUI::HybridTestTab &hybrid,
                          GUI::CalibrateTab &calibrate,
                          GUI::TBrowserTab &brow);
        ~MainView();

    public slots:
        void enableAllTabsSlot(const bool enable);

    private slots:
        void on_actionAbout_triggered();

        void on_actionUser_Settings_triggered();

    private:
        //void enableAllTabs(bool enable);
        Ui::MainView *ui;

    };
}
