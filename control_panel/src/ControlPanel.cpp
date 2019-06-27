#include "control/ControlPanel.h"

#include "control/Agent.h"
#include "control/Communication.h"

#include <essentials/IDManager.h>

#include <QApplication>
#include <QtWidgets>

#include <iostream>

namespace control
{

ControlPanel::ControlPanel()
{
    // COMMUNICATION
    this->comm = new Communication(this);

    // UI
    rootWidget_ = new QWidget();
    rootWidget_->setAttribute(Qt::WA_AlwaysShowToolTips, true);
    rootWidget_->setLayout(new QVBoxLayout());
    setCentralWidget(rootWidget_);

    // WORKER THREAD
    this->doWorkTimer = new QTimer();
    QObject::connect(doWorkTimer, SIGNAL(timeout()), this, SLOT(run()));
    this->doWorkTimer->start(200);

    this->idManager = new essentials::IDManager();
}

ControlPanel::~ControlPanel()
{
    delete this->doWorkTimer;
    delete this->idManager;
}

void ControlPanel::run()
{
    this->processMessage();

    this->updateUI();
}

void ControlPanel::processMessage()
{
    std::lock_guard<std::mutex> lck(this->queueMutex);

    while (!this->processStatsQueue.empty()) {
        auto timePstsPair = processStatsQueue.front();
        processStatsQueue.pop();
        Agent* agent = this->getAgent(timePstsPair.second.senderID);
        agent->update(timePstsPair);
        std::cout << "ControlPanel: processing Process Stats from " << agent->getID() << std::endl;
    }

    while (!this->alicaInfosQueue.empty()) {
        auto timeAlicaInfoPair = alicaInfosQueue.front();
        alicaInfosQueue.pop();
        Agent* agent = this->getAgent(timeAlicaInfoPair.second.senderID);
        std::cout << "ControlPanel: processing Alica Info from " << agent->getID() << std::endl;
    }
}

void ControlPanel::updateUI() {}

Agent* ControlPanel::getAgent(essentials::IdentifierConstPtr id)
{
    auto agentEntry = this->agents.find(id);
    if (agentEntry != this->agents.end()) {
        return agentEntry->second;
    } else {
        auto agentIter = this->agents.emplace(id, new Agent(id, this->rootWidget_->layout()));
        return agentIter.first->second;
    }
}

essentials::IDManager* ControlPanel::getIDManager() { return this->idManager; }

void ControlPanel::enqueue(process_manager::ProcessStats psts)
{
    std::lock_guard<std::mutex> lck(this->queueMutex);
    this->processStatsQueue.emplace(std::chrono::system_clock::now(), psts);
}

void ControlPanel::enqueue(alica::AlicaEngineInfo aei)
{
    std::lock_guard<std::mutex> lck(this->queueMutex);
    this->alicaInfosQueue.emplace(std::chrono::system_clock::now(), aei);
}

} // namespace control

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Distributed Systems Research Group");
    QCoreApplication::setApplicationName("Control Panel");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    control::ControlPanel mainWin;
    mainWin.show();
    return app.exec();
}