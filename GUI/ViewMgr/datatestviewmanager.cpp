#include "datatestviewmanager.h"
#include "View/datatesttab.h"
#include "Model/datatest.h"
#include <QDebug>
#include <QThread>

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
        WireThreads();
    }

    DataTestViewManager::~DataTestViewManager()
    {
        qDebug() << "Destructing " << this;

    }

    void DataTestViewManager::WireButtons()
    {
        connect(&m_dataTestTab, SIGNAL(notifyAddGraph()),
                &m_dataTest, SLOT(createGraph()));
    }

    void DataTestViewManager::WireThreads()
    {
        connect(&m_dataTest, SIGNAL(sendGraphData(std::vector<TH1F*>)),
                &m_dataTestTab, SLOT(drawGraph(std::vector<TH1F*>)));
    }


}
