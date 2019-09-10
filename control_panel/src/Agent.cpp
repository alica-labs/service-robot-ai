#include "control/Agent.h"

#include "control/ExecutableRegistry.h"
#include "control/Process.h"

//#include "/home/emmeda/Research/dev/rosws/devel/.private/control_panel/include/ui_Agent.h"
#include "ui_Agent.h"
#include <sstream>

namespace control
{
Agent::Agent(essentials::IdentifierConstPtr agentID, QLayout* parent, ExecutableRegistry* executableRegistry)
        : agentID(agentID)
        , uiAgent(new Ui::Agent())
        , agentGroupBox(new QGroupBox())
        , executableRegistry(executableRegistry)
{
    this->uiAgent->setupUi(this->agentGroupBox);
    parent->addWidget(this->agentGroupBox);

    essentials::SystemConfig* sc = essentials::SystemConfig::getInstance();
    this->msgTimeOut = std::chrono::duration<double>((*sc)["ProcessManaging"]->get<unsigned long>("Control.timeLastMsgReceivedTimeOut", NULL));
}

Agent::~Agent() {}

void Agent::updateName() {
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
            proc = new Process(this, ps.processKey, this->executableRegistry);
            this->processes.emplace(ps.processKey, proc);
        }
        proc->update(this->timeLastMsgReceived, ps);
    }
    updateName();
}

void Agent::addExec(QWidget* exec)
{
    this->uiAgent->verticalLayout->insertWidget(this->uiAgent->verticalLayout->count()-1, exec);
}
} // namespace control