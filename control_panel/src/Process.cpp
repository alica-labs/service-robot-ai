#include "control/Process.h"

#include "control/Agent.h"
#include "control/ExecutableRegistry.h"
#include "ui_Agent.h"
#include "ui_Process.h"

#include <SystemConfig.h>
#include <process_manager/ExecutableMetaData.h>
#include <process_manager/containers/ProcessCommand.h>
#include <qt5/QtWidgets/QMenu>

namespace control
{
const std::string Process::redBackground = "background-color:#FF4719;";
const std::string Process::greenBackground = "background-color:#66FF66;";
const std::string Process::grayBackground = "background-color:gray;";

Process::Process(Agent* parentAgent, int processKey, ExecutableRegistry* executableRegistry)
        : execMetaData(executableRegistry->getExecutable(processKey))
        , memory(0)
        , state('U')
        , cpu(0)
        , parentAgent(parentAgent)
        , runningParamSet(process_manager::ExecutableMetaData::UNKNOWN_PARAMS)
        , publishing(false)
        , uiProcess(new Ui::Process())
        , processWidget(new QWidget())
        , desiredParamSet(INT_MAX)
        , executableRegistry(executableRegistry)
{
    for (auto paramEntry : this->execMetaData->parameterMap) {
        if (this->desiredParamSet > paramEntry.first) {
            this->desiredParamSet = paramEntry.first;
        }
    }

    this->uiProcess->setupUi(this->processWidget);
    this->uiProcess->processName->setText(QString(this->execMetaData->name.c_str()));

    QObject::connect(this->uiProcess->checkBox, SIGNAL(stateChanged(int)), this, SLOT(handleCheckBoxStateChanged(int)), Qt::DirectConnection);

    this->processWidget->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    connect(this->processWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

    this->msgTimeOut = std::chrono::duration<double>(
            (*essentials::SystemConfig::getInstance())["ProcessManaging"]->get<unsigned long>("Control.timeLastMsgReceivedTimeOut", NULL));

    this->parentAgent->addExec(this->processWidget);
    this->processWidget->show();
}

Process::~Process() {}

void Process::showContextMenu(const QPoint& pos)
{
    /* HINT: remember, if there are some problems that way:
     * For QAbstractScrollArea and derived classes you would use:
     * QPoint globalPos = myWidget->viewport()->mapToGlobal(pos); */

    QPoint globalPos = this->processWidget->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Start publishing logs");
    myMenu.addAction("Stop publishing logs");

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem) {
        std::string selectedString = selectedItem->iconText().toStdString();
        if (selectedString == "Start publishing logs") {
            this->sendProcessCommand(process_manager::ProcessCommand::START_LOG_PUBLISHING);
        } else if (selectedString == "Stop publishing logs") {
            this->sendProcessCommand(process_manager::ProcessCommand::STOP_LOG_PUBLISHING);
        } else {
            std::cerr << "Process: Chosen context menu option is unhandled!" << std::endl;
        }
    } else {
        std::cout << "Process: Nothing chosen in context menu!" << std::endl;
    }
}

/**
 * Updates the informations about the process with the given information.
 * @param ps The given information about the process.
 */
void Process::update(std::chrono::system_clock::time_point timeMsgReceived, process_manager::ProcessStat ps)
{
    this->timeLastMsgReceived = timeMsgReceived;
    this->cpu = ps.cpu;
    this->memory = ps.mem;
    this->state = ps.state;
    this->runningParamSet = ps.paramSet;
    if (ps.publishing == process_manager::ProcessStat::PUBLISHING_ON) {
        this->publishing = true;
    } else if (ps.publishing == process_manager::ProcessStat::PUBLISHING_OFF) {
        this->publishing = false;
    } else {
        std::cerr << "Process: Unknown publishing flag for process " << this->execMetaData->name << std::endl;
    }
    auto entry = this->execMetaData->parameterMap.find(this->runningParamSet);
    if (entry != this->execMetaData->parameterMap.end()) {
        std::stringstream ss;
        ss << "Command: ";
        for (auto param : entry->second) {
            if (param != nullptr) {
                ss << param << " ";
            }
        }
        this->processWidget->setToolTip(QString(ss.str().c_str()));
    } else {
        this->processWidget->setToolTip(QString("Command: Unknown"));
    }
}

void Process::update(std::chrono::system_clock::time_point now)
{
    if ((now - this->timeLastMsgReceived) > this->msgTimeOut) { // time is over, set controlled executable to not running

        this->uiProcess->processName->setText(QString(this->execMetaData->name.c_str()));
        this->uiProcess->cpuState->setText(QString("C: -- %"));
        this->uiProcess->memState->setText(QString("M: -- MB"));
        this->runningParamSet = process_manager::ExecutableMetaData::UNKNOWN_PARAMS;
        this->processWidget->setToolTip(QString(""));
        this->processWidget->setStyleSheet(redBackground.c_str());
    } else { // message arrived before timeout, update its GUI
        if (this->publishing) {
            this->uiProcess->processName->setText(QString((this->execMetaData->name + " (L)").c_str()));
        } else {
            this->uiProcess->processName->setText(QString(this->execMetaData->name.c_str()));
        }
        QString cpuString = "C: " + QString::number(this->cpu) + " %";
        QString memString = "M: " + QString::number(this->memory) + " MB";
        this->uiProcess->cpuState->setText(cpuString);
        this->uiProcess->memState->setText(memString);

        switch (this->state) {
        case 'R': // running
        case 'S': // interruptable sleeping
        case 'D': // uninterruptable sleeping
        case 'W': // paging
            this->processWidget->setStyleSheet(greenBackground.c_str());
            break;
        case 'Z': // zombie
        case 'T': // traced, or stopped
            this->processWidget->setStyleSheet(redBackground.c_str());
            this->processWidget->setToolTip(QString(""));
            this->runningParamSet = process_manager::ExecutableMetaData::UNKNOWN_PARAMS;
            break;
        case 'U':
        default:
            std::cout << "Process: Unknown process state '" << this->state << "' encountered!" << std::endl;
            this->processWidget->setStyleSheet(grayBackground.c_str());
            break;
        }
    }
}

void Process::handleBundleComboBoxChanged(QString bundle)
{
    for (auto paramEntry : this->execMetaData->parameterMap) {
        if (this->desiredParamSet > paramEntry.first) {
            this->desiredParamSet = paramEntry.first;
        }
    }

    if (bundle == "ALL") {
        this->processWidget->show();
        this->uiProcess->checkBox->setEnabled(true);
        return;
    }

    if (bundle == "RUNNING") {
        switch (this->state) {
        case 'R': // running
        case 'S': // interruptable sleeping
        case 'D': // uninterruptable sleeping
        case 'W': // paging
        case 'Z': // zombie
            this->processWidget->show();
            this->uiProcess->checkBox->setEnabled(true);
            break;
        case 'T': // traced, or stopped
        case 'U': // unknown
        default:
            this->processWidget->hide();
            break;
        }
        return;
    }

    auto bundleMapEntry = this->executableRegistry->getBundlesMap()->find(bundle.toStdString());
    if (bundleMapEntry != this->executableRegistry->getBundlesMap()->end()) {
        bool found = false;
        for (auto processParamSetPair : bundleMapEntry->second) {
            if (this->execMetaData->id == processParamSetPair.first) {
                found = true;
                this->desiredParamSet = processParamSetPair.second;
                if (processParamSetPair.second == this->runningParamSet || this->runningParamSet == process_manager::ExecutableMetaData::UNKNOWN_PARAMS) {
                    this->uiProcess->checkBox->setEnabled(true);
                } else { // disable the checkbox, if the wrong bundle is selected
                    this->uiProcess->checkBox->setEnabled(false);
                }
            }
        }
        if (found) {
            this->processWidget->show();
        } else {
            this->processWidget->hide();
        }
    } else {
        std::cerr << "Process: Selected bundle " << bundle.toStdString() << " not found!" << std::endl;
    }
}

void Process::handleCheckBoxStateChanged(int newState)
{
    switch (newState) {
    case Qt::CheckState::Checked:
        this->sendProcessCommand(process_manager::ProcessCommand::START);
        return;
    case Qt::CheckState::Unchecked:
        this->sendProcessCommand(process_manager::ProcessCommand::STOP);
        return;
    case Qt::CheckState::PartiallyChecked:
        // should not happen!?
        std::cerr << "Process: What does it mean, that a process is partially checked?!" << std::endl;
        return;
    default:
        std::cerr << "Process: Unknown new state of a checkbox!" << std::endl;
        return;
    }
}

void Process::sendProcessCommand(int cmd)
{
    process_manager::ProcessCommand pc;
    pc.processKeys.push_back(this->execMetaData->id);
    pc.paramSets.push_back(this->desiredParamSet);
    std::cerr << "Process: Sending Process Command not integrated, yet!" << std::endl;
    //        this->parentAgent->getComm()->sendProcessCommand(pc);
}

} // namespace control
