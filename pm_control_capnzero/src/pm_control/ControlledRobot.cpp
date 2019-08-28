#include "pm_control/ControlledRobot.h"
#include "pm_control/ControlledExecutable.h"
#include "pm_control/ControlledProcessManager.h"
#include "pm_control/Communication.h"
#include "ui_ProcessWidget.h"
#include "ui_RobotProcessesWidget.h"

#include <SystemConfig.h>
#include <essentials/Identifier.h>
#include <essentials/WildcardID.h>
#include <process_manager/ExecutableMetaData.h>
#include <process_manager/containers/ProcessCommand.h>
#include <process_manager/RobotExecutableRegistry.h>

#include <limits.h>
#include <ros/ros.h>
namespace pm_control
{
// Second Constructor is for robot_control
ControlledRobot::ControlledRobot(string robotName, const essentials::Identifier* robotId, const essentials::Identifier* parentPMid, pm_control::Communication* comm)
        : RobotMetaData(robotName, robotId)
        , robotProcessesQFrame(new QFrame())
        , _robotProcessesWidget(new Ui::RobotProcessesWidget())
        , parentPMid(parentPMid)
        , comm(comm)
{
    // setup gui stuff
    this->_robotProcessesWidget->setupUi(this->robotProcessesQFrame);
    auto pmRegistry = process_manager::RobotExecutableRegistry::get();
    if (dynamic_cast<const essentials::WildcardID*>(parentPMid)) {
        // don't show in robot_control
        this->_robotProcessesWidget->robotHostLabel->hide();
        this->inRobotControl = true;
    } else {
        string pmName;
        if (pmRegistry->getRobotName(parentPMid, pmName)) {
            this->_robotProcessesWidget->robotHostLabel->setText(QString((robotName + " on " + pmName).c_str()));
        } else {
            this->_robotProcessesWidget->robotHostLabel->setText(QString((robotName + " on UNKNOWN").c_str()));
        }
        this->inRobotControl = false;
    }

    QObject::connect(this->_robotProcessesWidget->bundleComboBox, SIGNAL(activated(QString)), this, SLOT(updateBundles(QString)));

    // enter bundles in combo box
    for (auto bundleEntry : *pmRegistry->getBundlesMap()) {
        this->_robotProcessesWidget->bundleComboBox->insertItem(INT_MAX, QString(bundleEntry.first.c_str()), QVariant(bundleEntry.first.c_str()));
    }

    // construct all known executables
    const vector<process_manager::ExecutableMetaData*>& execMetaDatas = pmRegistry->getExecutables();
    ControlledExecutable* controlledExec;
    for (auto execMetaDataEntry : execMetaDatas) {
        controlledExec = new ControlledExecutable(execMetaDataEntry, this);
        this->controlledExecMap.emplace(execMetaDataEntry->id, controlledExec);
    }

    essentials::SystemConfig* sc = essentials::SystemConfig::getInstance();
    this->msgTimeOut = chrono::duration<double>((*sc)["ProcessManaging"]->get<unsigned long>("PMControl.timeLastMsgReceivedTimeOut", NULL));

    // TODO
//    ros::NodeHandle* nh = new ros::NodeHandle();
//    string cmdTopic = (*essentials::SystemConfig::getInstance())["ProcessManaging"]->get<string>("Topics.processCmdTopic", NULL);
//    processCommandPub = nh->advertise<process_manager::ProcessCommand>(cmdTopic, 10);
}

ControlledRobot::~ControlledRobot()
{
    for (auto execEntry : this->controlledExecMap) {
        delete execEntry.second;
    }
    delete robotProcessesQFrame;
}

void ControlledRobot::handleProcessStat(
        chrono::system_clock::time_point timeMsgReceived, process_manager::ProcessStat ps, const essentials::Identifier* parentPMid)
{
    this->parentPMid = parentPMid;

    auto controlledExecEntry = this->controlledExecMap.find(ps.processKey);
    if (controlledExecEntry != this->controlledExecMap.end()) { // executable is already known
        this->timeLastMsgReceived = timeMsgReceived;
        // update the statistics of the ControlledExecutable
        controlledExecEntry->second->handleStat(timeMsgReceived, ps);
    } else { // executable is unknown
        cerr << "ControlledRobot: Received processStat for unknown executable with process key " << ps.processKey << endl;
        return;
    }
}

void ControlledRobot::updateGUI(chrono::system_clock::time_point now)
{
    if ((now - this->timeLastMsgReceived) > this->msgTimeOut && !inRobotControl) { // time is over, erase controlled robot
        this->robotProcessesQFrame->hide();
    } else {
        this->robotProcessesQFrame->show();
    }

    for (auto controlledExecEntry : this->controlledExecMap) {
        controlledExecEntry.second->updateGUI(now);
    }
}

void ControlledRobot::updateBundles(QString text)
{
    for (auto controlledExecMapEntry : this->controlledExecMap) {
        controlledExecMapEntry.second->handleBundleComboBoxChanged(text);
    }
}

void ControlledRobot::addExec(QWidget* exec)
{
    this->_robotProcessesWidget->verticalLayout->insertWidget(2, exec);
}

void ControlledRobot::removeExec(QWidget* exec)
{
    this->_robotProcessesWidget->verticalLayout->removeWidget(exec);
}

void ControlledRobot::sendProcessCommand(process_manager::ProcessCommand pc)
{
    pc.receiverID = this->parentPMid;
    pc.robotIDs.push_back(this->agentID);
    this->comm->sendProcessCommand(pc);
}
} /* namespace pm_widget */