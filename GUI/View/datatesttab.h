#ifndef DATATESTTAB_H
#define DATATESTTAB_H

#include <QWidget>
#include "lib/qcustomplot.h"

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
        void setGraphs(QCPBars *plot);


    private slots:
        void on_btnStart_clicked();

    private:
        Ui::DataTestTab *ui;
    };

}
#endif // DATATESTTAB_H
