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
        WireCanvas();
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

        connect(&m_dataTest, SIGNAL(startedDataTest()),
                &m_dataTestTab, SLOT(onDataTestStart()));
        connect(&m_dataTest, SIGNAL(finishedDataTest()),
                &m_dataTestTab, SLOT(onDataTestFinish()));

        connect(&m_dataTest, SIGNAL(sendGraphData(std::vector<std::shared_ptr<TH1D> >)),
                &m_dataTestTab, SLOT(drawGraph(std::vector<std::shared_ptr<TH1D> >)));
    }

    void DataTestViewManager::WireCanvas()
    {
        connect(&m_dataTest, SIGNAL(getTCanvas()),
                &m_dataTestTab, SLOT(getTCanvas()));
        connect(&m_dataTestTab, SIGNAL(sendTCanvas(std::vector<TCanvas*>)),
                &m_dataTest, SLOT(recieveTCanvas(std::vector<TCanvas*>)));
        connect(&m_dataTest, SIGNAL(sendRefresh()),
                &m_dataTestTab, SLOT(refreshTCanvas()));
        connect(this, SIGNAL(on2CbcToggle(bool)),
                &m_dataTestTab, SLOT(setupCanvas(bool)));

    }

    void DataTestViewManager::WireExternalCalls()
    {
     //connect(m_dataTest,)
        connect(&m_dataTest, SIGNAL(sendAccept(HwDescriptionVisitor)),
                this, SIGNAL(sendAccept(HwDescriptionVisitor)));

    }



}
