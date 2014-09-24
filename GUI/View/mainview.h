#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>

namespace Ui {
    class MainView;
}

namespace GUI{

    class SetupTab;
    class CbcRegistersTab;
    class DataTestTab;

    class MainView : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainView(QWidget *parent,
                          GUI::SetupTab &setup,
                          GUI::CbcRegistersTab &reg,
                          GUI::DataTestTab &data);
        ~MainView();

    private:
        Ui::MainView *ui;

    };
}

#endif // MAINVIEW_H
