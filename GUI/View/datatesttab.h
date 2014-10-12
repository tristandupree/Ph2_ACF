#ifndef DATATESTTAB_H
#define DATATESTTAB_H

#include <QWidget>
#include "lib/TQtWidget.h"
#include "TH1D.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <memory>

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
        void drawGraph(const std::vector<TH1D *> hists);


    private slots:
        void on_btnStart_clicked();

    private:
        Ui::DataTestTab *ui;
        std::vector<TQtWidget*> m_vectorCanvas;
        std::vector<QGroupBox*> m_vectorGroupBox;
        std::vector<QHBoxLayout*> m_vectorLayout;

        void setupCanvas(int cNCbc);
        void drawTest();
    };

}
#endif // DATATESTTAB_H
