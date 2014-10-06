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
    signals:
        //void onBtnCustomClicked();

    public slots:
        void tabEnableRegisters(bool enable);
        void tabEnableDataTest(bool enable);
        void tabEnableMacroTest(bool enable);

    private slots:
        void on_actionAbout_triggered();

        void on_actionUser_Settings_triggered();

    private:
        Ui::MainView *ui;

    };
}

#endif // MAINVIEW_H
