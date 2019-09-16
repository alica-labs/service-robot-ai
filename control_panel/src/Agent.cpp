#include "control/Agent.h"

#include "control/Communication.h"
#include "control/ControlPanel.h"
#include "control/ExecutableRegistry.h"
#include "control/Process.h"

#include "ui_Agent.h"

#include <sstream>

namespace control
{
Agent::Agent(essentials::IdentifierConstPtr agentID, control::ControlPanel* controlPanel)
        : agentID(agentID)
        , controlPanel(controlPanel)
        , uiAgent(new Ui::Agent())
        , agentGroupBox(new QGroupBox())
{
    // add agent to parent gui object
    this->uiAgent->setupUi(this->agentGroupBox);
    this->controlPanel->rootWidget_->layout()->addWidget(this->agentGroupBox);

    essentials::SystemConfig* sc = essentials::SystemConfig::getInstance();
    this->msgTimeOut = std::chrono::duration<double>((*sc)["ProcessManaging"]->get<unsigned long>("Control.timeLastMsgReceivedTimeOut", NULL));
    QObject::connect(this->uiAgent->agentCommandBtn, SIGNAL(clicked(bool)), this, SLOT(handleAgentCommandBtnClicked(bool)), Qt::DirectConnection);
}

Agent::~Agent() {}

void Agent::updateName()
{
    std::stringstream ss;
    if (hostID.get() != nullptr) {
        ss << (*agentID.get()) << " on " << (*hostID.get());
    } else {
        ss << (*agentID.get());
    }
    this->agentGroupBox->setTitle(QString(ss.str().c_str()));
}

essentials::IdentifierConstPtr Agent::getAgentID()
{
    return this->agentID;
}

void Agent::update(std::chrono::system_clock::time_point now)
{
    if ((now - this->timeLastMsgReceived) > this->msgTimeOut) {
        this->agentGroupBox->hide();
    } else {
        this->agentGroupBox->show();
    }

    for (auto procEntry : this->processes) {
        procEntry.second->update(now);
    }
}

void Agent::update(std::pair<std::chrono::system_clock::time_point, process_manager::ProcessStats> timePstsPair)
{
    this->timeLastMsgReceived = timePstsPair.first;
    this->hostID = timePstsPair.second.senderID;
    for (process_manager::ProcessStat ps : timePstsPair.second.processStats) {
        Process* proc;
        auto processEntry = this->processes.find(ps.processKey);
        if (processEntry != this->processes.end()) {
            proc = processEntry->second;
        } else {
            proc = new Process(this, ps.processKey, this->controlPanel->getExecutableRegistry());
            this->processes.emplace(ps.processKey, proc);
        }
        proc->update(this->timeLastMsgReceived, ps);
    }
    updateName();
}

void Agent::addExec(QWidget* exec)
{
    this->uiAgent->verticalLayout->insertWidget(this->uiAgent->verticalLayout->count() - 1, exec);
}

void Agent::handleAgentCommandBtnClicked(bool checked)
{
    AgentCommand ac;
    ac.receiverID = this->getAgentID();
    if (checked) {
        ac.cmd = AgentCommand::START;
    } else {
        ac.cmd = AgentCommand::STOP;
    }
    this->controlPanel->getCommunication()->send(ac);
}
} // namespace control