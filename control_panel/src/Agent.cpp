#include "control/Agent.h"

#include "control/Communication.h"
#include "control/ControlPanel.h"
#include "control/ExecutableRegistry.h"
#include "control/Process.h"

#include "ui_Agent.h"

#include <essentials/IDManager.h>

#include <sstream>

namespace control
{
Agent::Agent(essentials::IdentifierConstPtr agentID, control::ControlPanel* controlPanel)
        : agentID(agentID)
        , controlPanel(controlPanel)
        , uiAgent(new Ui::Agent())
        , agentGroupBox(new QGroupBox())
        , shown(false)
{
    // add agent to parent gui object
    this->uiAgent->setupUi(this->agentGroupBox);
    if (!shown) {
        this->agentGroupBox->hide();
    }
    this->controlPanel->rootWidget_->layout()->addWidget(this->agentGroupBox);

    // set agentName to string version of ID
    std::stringstream ss;
    ss << (*agentID.get());
    this->agentName = ss.str();

    essentials::SystemConfig* sc = essentials::SystemConfig::getInstance();
    this->msgTimeOut = std::chrono::duration<double>((*sc)["ProcessManaging"]->get<unsigned long>("Control.timeLastMsgReceivedTimeOut", NULL));
    QObject::connect(this->uiAgent->agentCommandBtn, SIGNAL(clicked(bool)), this, SLOT(handleAgentCommandBtnClicked(bool)), Qt::DirectConnection);
}

Agent::Agent(essentials::IdentifierConstPtr id, std::string agentName, control::ControlPanel *controlPanel) : Agent(id, controlPanel){
    this->agentName = agentName;
}

Agent::~Agent() {}

std::string Agent::getName()
{
    return this->agentName;
}

essentials::IdentifierConstPtr Agent::getAgentID()
{
    return this->agentID;
}

void Agent::updateGUI(std::chrono::system_clock::time_point now)
{
//    if ((now - this->timeLastMsgReceived) > this->msgTimeOut) {
//        this->agentGroupBox->hide();
//    } else {
//        this->agentGroupBox->show();
//    }

    for (auto procEntry : this->processes) {
        procEntry.second->update(now);
    }

    std::stringstream ss;
    if (hostID.get() != nullptr) {
        ss << this->agentName << " on " << (*hostID.get());
    } else {
        ss << this->agentName;
    }

    this->agentGroupBox->setTitle(QString(ss.str().c_str()));
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
        this->uiAgent->agentCommandBtn->setText(QString("Stop"));
    } else {
        ac.cmd = AgentCommand::STOP;
        this->uiAgent->agentCommandBtn->setText(QString("Start"));
    }
    this->controlPanel->getCommunication()->send(ac);
}

void Agent::toggle()
{
    if (shown) {
        this->hide();
    } else {
        this->show();
    }
}

bool Agent::isShown()
{
    return this->shown;
}

void Agent::show()
{
    this->shown = true;
    this->agentGroupBox->show();
}

void Agent::hide()
{
    this->shown = false;
    this->agentGroupBox->hide();
}
} // namespace control