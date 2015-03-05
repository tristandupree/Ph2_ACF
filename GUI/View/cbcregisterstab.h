#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QTableView>
#include <map>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include "Model/systemcontroller.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

namespace Ui {
    class CbcRegistersTab;
}

namespace GUI {

    class CbcRegistersTab : public QWidget
    {
        Q_OBJECT

    public:
        explicit CbcRegistersTab(QWidget *parent);
        ~CbcRegistersTab();

    signals:
        void refreshCbcRegisters();
        void sendCbcRegisters(const int cbc, std::vector<std::pair<std::string, std::uint8_t>>);

    public slots:
        void setupCbcRegGrid(const bool cbc2);
        void createCbcRegisterValue(const int cbc, const std::map<std::string, CbcRegItem> mapReg);

    private slots:

        void on_btnRefresh_clicked();

        void on_btnUpdate_clicked();

    private:
        Ui::CbcRegistersTab *ui;

        std::vector<std::vector<QGridLayout*>> m_loGridVec; //so I can access widgets inside it later
        QVector<QMap<QString, QMap<QLabel*, QLineEdit*>>> m_widgetMap; //vector of CBCs access the map via the name of the register

        QTabWidget *m_tabCbc;

        QTabWidget *createCbcTab();
        void createCbcRegItems();
        void clearTabs();
    };
}

