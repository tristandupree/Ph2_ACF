#include "datatestviewmanager.h"
#include "View/datatesttab.h"
#include "Model/datatest.h"
#include <QDebug>
#include <QThread>
#include "TH1D.h"

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
        connect(&m_dataTest, SIGNAL(getVcthValue()),
                &m_dataTestTab, SLOT(getVcthDialValue()));
        connect(&m_dataTestTab, SIGNAL(sendVcthValue(int)),
                &m_dataTest, SLOT(setVcthValue(int)));
        connect(&m_dataTest, SIGNAL(getEventsValue()),
                &m_dataTestTab, SLOT(getEventsDial()));
        connect(&m_dataTestTab, SIGNAL(sendEventsNumber(int)),
                &m_dataTest, SLOT(setEventsValue(int)));
    }

    void DataTestViewManager::WireThreads()
    {
        connect(&m_dataTest, SIGNAL(sendGraphData(std::vector<std::shared_ptr<TH1D>>)),
                &m_dataTestTab, SLOT(drawGraph(std::vector<std::shared_ptr<TH1D>>)));
    }


}
