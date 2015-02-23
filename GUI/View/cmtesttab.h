#pragma once
#include <QWidget>

namespace Ui {
    class CmTestTab;
}

namespace GUI {


    class CmTestTab : public QWidget
    {
        Q_OBJECT

    public:
        explicit CmTestTab(QWidget *parent);
        ~CmTestTab();

    private:
        Ui::CmTestTab *ui;
    };
}

