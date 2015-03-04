#pragma once
#include <QDebug>
#include <QThread>
#include "Model/systemcontrollerworker.h"
#include "../System/SystemController.h"

//TODO Add proper destruct and safety methods to thread

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

namespace GUI
{

    SystemControllerWorker::SystemControllerWorker(QObject *parent,
                                                   Settings &config) :
        QObject(parent)
        //m_Settings(config)
    {
        _working = false;
        _abort = false;
    }

    SystemControllerWorker::~SystemControllerWorker()
    {
        qDebug() << "Destructing SystemController Worker" << this;
    }

    void SystemControllerWorker::requestWork(std::string cHwFile)
    {
        mutex.lock();
        _working = true;
        _abort = false;
        qDebug() << "Starting SystemController Worker on thread " << this;
        mutex.unlock();

        emit workRequested();
    }

    void SystemControllerWorker::requestConfigureHw()
    {
        mutex.lock();
        _working = true;
        _abort = false;
        mutex.unlock();

        emit workConfigureHwRequested();
    }

    void SystemControllerWorker::abort()
    {
        mutex.lock();
        if (_working) {
            _abort = true;
            qDebug()<<"SystemController worker aborting in Thread "<<thread()->currentThreadId();
        }
        mutex.unlock();
    }

    void SystemControllerWorker::onInitialiseHw()
    {
        mutex.lock();
        bool abort = _abort;
        mutex.unlock();

        if (abort) {
            return;
        }
        InitializeHw("../settings/HWDescription_2CBC2.json");
        //InitializeHw(m_HwFile);

        mutex.lock();
        _working = false;
        mutex.unlock();
        emit finishedInitialiseHw();
    }

    void SystemControllerWorker::onConfigureHw()
    {
        qDebug() << "Configuring hardware on: " << this;

        ConfigureHw();

        mutex.lock();
        _working = false;
        mutex.unlock();
        emit finishedConfigureHw();
        qDebug() << "Finished configure";
    }

}
