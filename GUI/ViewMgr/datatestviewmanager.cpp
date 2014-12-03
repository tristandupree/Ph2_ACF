#include "datatestviewmanager.h"
#include "View/datatesttab.h"
#include "Model/datatest.h"
#include <QDebug>
#include <QThread>
#include "TH1F.h"

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
                &m_dataTest, SLOT(initialiseSettings()));
        connect(&m_dataTest, SIGNAL(getVcthValue()),
                &m_dataTestTab, SLOT(getVcthDialValue()));
        connect(&m_dataTestTab, SIGNAL(sendVcthValue(int)),
                &m_dataTest, SLOT(setVcthValue(int)));
        connect(&m_dataTest, SIGNAL(getEventsValue()),
                &m_dataTestTab, SLOT(getEventsDial()));
        connect(&m_dataTestTab, SIGNAL(sendEventsNumber(int)),
                &m_dataTest, SLOT(setEventsValue(int)));
        connect(&m_dataTest, SIGNAL(getIsRegTestChecked()),
                &m_dataTestTab, SLOT(getIsRegTestChecked()));
        connect(&m_dataTest, SIGNAL(getIsScanChecked()),
                &m_dataTestTab, SLOT(getIsScanChecked()));
        connect(&m_dataTestTab, SIGNAL(sendIsRegTestChecked(bool)),
                &m_dataTest, SLOT(setTestReg(bool)));
        connect(&m_dataTestTab, SIGNAL(sendIsScanChecked(bool)),
                &m_dataTest, SLOT(setScanThreshold(bool)));
        connect(&m_dataTest, SIGNAL(startedDataTest()),
                &m_dataTestTab, SLOT(onDataTestStart()));
        connect(&m_dataTest, SIGNAL(finishedDataTest()),
                &m_dataTestTab, SLOT(onDataTestFinish()));
        connect(&m_dataTest, SIGNAL(getIsHoleModeChecked()),
                &m_dataTestTab, SLOT(getIsHoleModeChecked()));
        connect(&m_dataTestTab, SIGNAL(sendIsHoleModeChecked(bool)),
                &m_dataTest, SLOT(setHoleMode(bool)));
    }

    void DataTestViewManager::WireCanvas()
    {
        connect(this, SIGNAL(on2CbcToggle(bool)),
                &m_dataTestTab, SLOT(setupCanvas(bool)));
        connect(&m_dataTest, SIGNAL(sendGraphData(std::vector<std::shared_ptr<TH1F> >)),
                &m_dataTestTab, SLOT(drawOccupancy(std::vector<std::shared_ptr<TH1F> >)));
        connect(&m_dataTest, SIGNAL(sendHistsThreshold(std::vector<std::shared_ptr<TH1F> >,std::string)),
                &m_dataTestTab, SLOT(drawThreshold(std::vector<std::shared_ptr<TH1F> >,std::string)));
        connect(&m_dataTest, SIGNAL(sendFitThreshold(std::vector<std::shared_ptr<TF1> >,std::string)),
                &m_dataTestTab, SLOT(drawFitThreshold(std::vector<std::shared_ptr<TF1> >,std::string)));

        connect(&m_dataTest, SIGNAL(sendHists(std::map<Cbc*,TH1F*>,std::string)),
                &m_dataTestTab, SLOT(receiveHists(std::map<Cbc*,TH1F*>,std::string)));
        connect(&m_dataTest, SIGNAL(sendRefreshHists()),
                &m_dataTestTab, SLOT(receiveRefreshHists()));
    }
}
