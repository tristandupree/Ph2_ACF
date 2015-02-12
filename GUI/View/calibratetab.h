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

    private:
        Ui::CalibrateTab *ui;
    };
}

