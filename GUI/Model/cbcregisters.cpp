#include "cbcregisters.h"
#include "Model/systemcontroller.h"

namespace GUI
{
    CbcRegisters::CbcRegisters(QObject *parent,
                               SystemController &sysCtrl) :
        QObject(parent),
        m_systemController(sysCtrl)
    {
        //m_systemControllerWorker = m_systemController.getSystemControllerWorker();
    }

    void getCbcRegistersMap()
    {
        //m_systemController;
    }
}
