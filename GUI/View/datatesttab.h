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
        void drawGraph(const QVector<double> &x, const QVector<double> &y);


    private slots:
        void on_btnStart_clicked();

    private:
        Ui::DataTestTab *ui;

        QCPBars *m_dataTestHist;
        void setupCanvas();
    };

}
#endif // DATATESTTAB_H
