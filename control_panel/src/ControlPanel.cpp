#include "control/ControlPanel.h"

#include "ui_ControlPanel.h"
#include "control/Agent.h"
#include "control/Talker.h"
#include "control/Communication.h"
#include "control/ExecutableRegistry.h"

#include <essentials/IDManager.h>

#include <QApplication>
#include <QtWidgets>

#include <iostream>

namespace control
{

ControlPanel::ControlPanel()
        : sc(essentials::SystemConfig::getInstance())
        , uiControlPanel(new Ui::ControlPanel())
        , controlPanelQWidget(new QWidget())
{



    // COMMUNICATION
    this->comm = new Communication(this);

    // UI
    this->uiControlPanel->setupUi(this->controlPanelQWidget);
    this->controlPanelQWidget->setAttribute(Qt::WA_AlwaysShowToolTips, true);
    this->controlPanelQWidget->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    QObject::connect(this->controlPanelQWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
    setCentralWidget(this->controlPanelQWidget);

    // Create Talker
    this->talker = new Talker(this);

    // Register robots from Globals.conf
    this->idManager = new essentials::IDManager();
    essentials::Configuration* globalsConf = (*sc)["Globals"];
    if (globalsConf != nullptr) {
        auto agentNames = globalsConf->getSections("Globals.Team", NULL);
        for (auto agentName : (*agentNames)) {
            int tmpId = globalsConf->get<int>("Globals.Team", agentName.c_str(),"ID", NULL);
            Agent* agent = new Agent(essentials::IdentifierConstPtr(this->idManager->getID(tmpId)), agentName, this);
            this->agents.emplace(agent->getAgentID(), agent);
        }
    }

    // Register executables from ProcessManaging.conf
    this->executableRegistry = ExecutableRegistry::get();
    auto processDescriptions = (*this->sc)["ProcessManaging"]->getSections("Processes.ProcessDescriptions", NULL);
    for (auto processSectionName : (*processDescriptions)) {
        this->executableRegistry->addExecutable(processSectionName);
    }

    // WORKER THREAD
    this->doWorkTimer = new QTimer();
    QObject::connect(doWorkTimer, SIGNAL(timeout()), this, SLOT(run()));
    this->doWorkTimer->start(200);
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

void ControlPanel::showContextMenu(const QPoint& pos)
{
    // create menu
    QMenu myMenu;
    for (auto& agent : this->agents) {
        std::stringstream ss;
        ss << *(agent.second->getAgentID());
        QIcon icon;
        if (this->agents[agent.first]->isShown()) {
            icon = QIcon::fromTheme("user-available", QIcon("user-available"));
        } else {
            icon = QIcon::fromTheme("user-offline", QIcon("user-offline"));
        }
        QAction* action = myMenu.addAction(icon, std::string(agent.second->getName() + " (" + ss.str() + ")").c_str());
        QObject::connect(action, &QAction::triggered, agent.second, &Agent::toggle);
    }

    /* HINT: remember, if there are some problems that way:
     * For QAbstractScrollArea and derived classes you would use:
     * QPoint globalPos = myWidget->viewport()->mapToGlobal(pos); */
    QPoint globalPos = this->controlPanelQWidget->mapToGlobal(pos);
    myMenu.exec(globalPos);
}

void ControlPanel::processMessage()
{
    std::lock_guard<std::mutex> lck(this->queueMutex);

    while (!this->processStatsQueue.empty()) {
        auto timePstsPair = processStatsQueue.front();
        processStatsQueue.pop();
        Agent* agent = this->getAgent(timePstsPair.second.senderID);
        agent->update(timePstsPair);
    }

    while (!this->alicaInfosQueue.empty()) {
        auto timeAlicaInfoPair = alicaInfosQueue.front();
        alicaInfosQueue.pop();
        Agent* agent = this->getAgent(timeAlicaInfoPair.second.senderID);
        agent->update(timeAlicaInfoPair);
    }
}

void ControlPanel::updateUI()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    for (auto agentEntry : this->agents) {
        agentEntry.second->updateGUI(now);
    }
}

Agent* ControlPanel::getAgent(essentials::IdentifierConstPtr id)
{
    auto agentEntry = this->agents.find(id);
    if (agentEntry != this->agents.end()) {
        return agentEntry->second;
    } else {
        auto agentIter = this->agents.emplace(id, new Agent(id, this));
        return agentIter.first->second;
    }
}

essentials::IDManager* ControlPanel::getIDManager()
{
    return this->idManager;
}

ExecutableRegistry* ControlPanel::getExecutableRegistry()
{
    return this->executableRegistry;
}

Communication* ControlPanel::getCommunication()
{
    return this->comm;
}

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