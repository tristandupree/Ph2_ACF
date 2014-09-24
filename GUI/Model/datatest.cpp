#include "datatest.h"

#include "Model/systemcontroller.h"

#include "lib/qcustomplot.h"
#include <QVector>

#include "../HWDescription/Cbc.h"
#include "../HWDescription/Module.h"
#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../HWInterface/Utilities.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

namespace GUI
{
    DataTest::DataTest(QObject *parent,
                       SystemController& sysCtrl) :
        QObject(parent),
        m_systemController(sysCtrl)
    {
    }

    DataTest::~DataTest()
    {

        qDebug() << "Destructing " << this;

    }

    void DataTest::createGraph()
    {
        qDebug() << "Button clicked";

        emit sendGraph(runTest());
    }

    QCPBars* DataTest::runTest()
    {

        QCustomPlot *customPlot = new QCustomPlot();

        QVector<QCPBars*> cHistVec;

        customPlot->plotLayout()->clear();
        QCPAxis *x = customPlot->axisRect()->axis(QCPAxis::atBottom);
        QCPAxis *y = customPlot->axisRect()->axis(QCPAxis::atRight);
        //cHistVec.push_back(new QCPBars(x, y));

        QCPBars *a = new QCPBars(x,y);



        return a;

        //for(uint8_t cNCbc=0; cNCbc<cSystemController.fShelveVec[0]->getBoard(0)->getModule(0)->getNCbc(); cNCbc++)
        //{
        //   cHistVec.push_back(new TH1F(Form("Histo_Hits_CBC%d",cNCbc), Form("Occupancy_CBC%d",cNCbc), 255, -0.5, 254.5));
        //}


    }


}
