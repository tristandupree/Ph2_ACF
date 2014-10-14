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

    class MacroTestTab; //cheap tab

    class MainView : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainView(QWidget *parent,
                          GUI::SetupTab &setup,
                          GUI::CbcRegistersTab &reg,
                          GUI::DataTestTab &data,
                          GUI::MacroTestTab &macroTest);
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

#endif // MAINVIEW_H
