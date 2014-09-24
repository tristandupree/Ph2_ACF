#include "datatestviewmanager.h"
#include "View/datatesttab.h"
#include "Model/datatest.h"
#include <QDebug>

namespace GUI
{
    DataTestViewManager::DataTestViewManager(QObject *parent,
                                             DataTestTab& dataTab,
                                             DataTest& dataTest) :
        QObject(parent),
        m_dataTestTab(dataTab),
        m_dataTest(dataTest)
    {
        WireButtons();
    }

    DataTestViewManager::~DataTestViewManager()
    {
        qDebug() << "Destructing " << this;
    }

    void DataTestViewManager::WireButtons()
    {
        connect(&m_dataTestTab, SIGNAL(notifyAddGraph()),
                &m_dataTest, SLOT(createGraph()));

        connect(&m_dataTest, SIGNAL(sendGraph(QCPBars*)),
                &m_dataTestTab, SLOT(setGraphs(QCPBars*)));
    }


}
