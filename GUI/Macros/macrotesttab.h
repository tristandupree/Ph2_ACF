#ifndef MACROTESTTAB_H
#define MACROTESTTAB_H

#include <QWidget>

namespace Ui {
    class MacroTestTab;
}

namespace GUI {

    class MacroTestTab : public QWidget
    {
        Q_OBJECT

    public:
        explicit MacroTestTab(QWidget *parent);
        ~MacroTestTab();

    private slots:
        void on_btnCalib_clicked();

        void on_btnReadData_clicked();

        void on_btnSystemTest_clicked();

        void on_btnInterface_clicked();

    private:
        Ui::MacroTestTab *ui;
    };
}
#endif // MACROTESTTAB_H
