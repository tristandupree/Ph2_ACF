#pragma once

#include <QWidget>


namespace Ui {
    class TBrowserTab;
}

namespace GUI{

    class TBrowserTab : public QWidget
    {
        Q_OBJECT

    public:
        explicit TBrowserTab(QWidget *parent);
        ~TBrowserTab();

    private slots:

        void on_Calibrate_clicked();

        void on_CmTest_clicked();

        void on_DataTest_clicked();

        void on_HybridTest_clicked();

        void on_InterfaceTest_clicked();

        void on_TBrowser_clicked();

    private:
        Ui::TBrowserTab *ui;
    };
}
