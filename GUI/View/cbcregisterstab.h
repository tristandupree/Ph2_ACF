#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QGridLayout>
#include <map>

namespace Ui {
    class CbcRegistersTab;
}

namespace GUI {

    struct CbcRegItem
    {
        int fPage;
        int fAddress;
        int fValue;
    };

    class QStandardItem;

    class CbcRegistersTab : public QWidget
    {
        Q_OBJECT

    public:
        explicit CbcRegistersTab(QWidget *parent);
        ~CbcRegistersTab();

    public slots:
        void setupCbcRegGrid(const bool cbc2);
        void createCbcRegisterValue(const int cbc, const std::map<std::string, CbcRegItem> mapReg);

    private:
        Ui::CbcRegistersTab *ui;

        std::vector<std::vector<QGridLayout*>> m_loGridVec; //so I can access widgets inside it later
        std::vector<std::vector<CbcRegItem>> m_cbcRegisterValues; //cbc, regItem

        QTabWidget *m_tabCbc;

        QTabWidget *createCbcTab();
    };
}

