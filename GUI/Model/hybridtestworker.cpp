#pragma once
#include "hybridtestworker.h"
#include <QThread>
#include <QDebug>
#include <QProcess>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include "../HWInterface/BeBoardFWInterface.h"
#include "../HWDescription/Shelve.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../Utils/Visitor.h"

using namespace boost::interprocess;
using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

namespace GUI {

    HybridTestWorker::HybridTestWorker(QObject *parent) :
        QObject(parent)
    {

    }

    HybridTestWorker::~HybridTestWorker()
    {
        qDebug() << "Destructing HybridTest Worker" << this;
    }

    void HybridTestWorker::doWork()
    {

        /*shared_memory_object::remove("Boost");
    	managed_shared_memory managed_shm{open_or_create, "Boost", 1024};

    	int *i = managed_shm.construct<int>("Integer")[10](91);
    	  std::cout << *i << '\n';
    	  std::pair<int*, std::size_t> p = managed_shm.find<int>("Integer");
    	  if (p.first)
    	  {
    	    std::cout << *p.first << '\n';
    	    std::cout << p.second << '\n';
          }*/

        managed_shared_memory shm(open_only, "HwDescriptionObjects");

        QProcess *process = new QProcess(this);
        QString cmd = QString("./bin/hybridtest --gui");

        cmd = cmd + " -v " + QString::number(m_Vcth);
        cmd = cmd + " -n " + QString::number(m_Events);

        cmd = m_HoleMode ? cmd + " --hm" : cmd;

        cmd = m_ScanThreshold ? cmd + " -s" : cmd;

        qDebug() << cmd;

        process->start(cmd);

        QByteArray ba;

        if (process->waitForStarted(-1)) {
            while(process->waitForReadyRead(-1)) {
                ba += process->readAllStandardOutput();
            }
        }

        char *someData = ba.data();

        qDebug() << "Testing QProcess ..." ;
        qDebug() << someData;
        qDebug() << "done!" ;

        emit finished();
    }

    void HybridTestWorker::requestWork(int vcth,int events, bool testreg,bool scanthreshold, bool holemode)
    {
        m_Vcth = vcth;
        m_Events = events;
        m_TestReg = testreg;
        m_ScanThreshold = scanthreshold;
        m_HoleMode = holemode;
        emit workRequested();
    }
}

