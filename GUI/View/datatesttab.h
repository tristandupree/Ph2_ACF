#ifndef DATATESTTAB_H
#define DATATESTTAB_H

#include <QWidget>
#include "TH1.h"
//#include "lib/qcustomplot.h"
//#include "lib/CustomTQtWidget.h"
//#include "lib/TQtWidget.h"

namespace Ui {
    class DataTestTab;
}

namespace GUI{

    //class QCPAbstractPlottable;

    class DataTestTab : public QWidget
    {
        Q_OBJECT

    public:
        explicit DataTestTab(QWidget *parent);

    ~DataTestTab();
    signals:
        void notifyAddGraph();

    public slots:
        void drawGraph(const std::vector<TH1F*> &value);


    private slots:
        void on_btnStart_clicked();

    private:
        Ui::DataTestTab *ui;
        //TH1D* h1;

        void setupCanvas(QString graphTitle, int nCbc);
        void drawTest();
    };

}
#endif // DATATESTTAB_H
