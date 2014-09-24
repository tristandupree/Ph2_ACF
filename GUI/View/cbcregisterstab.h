#ifndef CBCREGISTERSTAB_H
#define CBCREGISTERSTAB_H

#include <QWidget>
#include <QTableView>
#include <QItemDelegate>
#include <QStandardItemModel>

namespace Ui {
    class CbcRegistersTab;
}

namespace GUI {

    class QStandardItem;

    class CbcRegistersTab : public QWidget
    {
        Q_OBJECT

    public:
        explicit CbcRegistersTab(QWidget *parent);
    ~CbcRegistersTab();

    private:
        Ui::CbcRegistersTab *ui;
        QStandardItemModel *model;
        void startup();
    };
}

#endif // CBCREGISTERSTAB_H
