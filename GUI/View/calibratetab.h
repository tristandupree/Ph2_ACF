#pragma once

#include <QWidget>

namespace Ui {
    class CalibrateTab;
}

namespace GUI{
    class CalibrateTab : public QWidget
    {
        Q_OBJECT

    public:
        explicit CalibrateTab(QWidget *parent);
        ~CalibrateTab();

    signals:
        void startCalibration();
        void sendIsFastCalib(bool isFast);
        void sendIsScan(bool isScan);
        void sendIsBitwise(bool isBit);
        void sendIsAllCh(bool isAllCh);

    public slots:
        void getIsFastCalib();
        void getIsScan();
        void getIsBitwise();
        void getIsAllCh();
        void enableLaunch();
        void disableLaunch();

    private slots:
        void on_btnCalibrate_clicked();

    private:
        Ui::CalibrateTab *ui;
    };
}

