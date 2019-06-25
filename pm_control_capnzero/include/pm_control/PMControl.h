#pragma once

#include "ControlledRobot.h"

#include <essentials/Identifier.h>
#include <process_manager/containers/ProcessStat.h>
#include <process_manager/containers/ProcessStats.h>
#include <ui_PMControl.h>

#include <capnzero/CapnZero.h>

#include <QDialog>
#include <QWidget>
#include <QtGui>
#include <QMainWindow>

#include <chrono>
#include <mutex>
#include <queue>
#include <utility>

namespace essentials
{
class SystemConfig;
} // namespace  essentials

namespace process_manager
{
class RobotExecutableRegistry;
}

namespace capnzero
{
class Subscriber;
class Publisher;
} // namespace capnzero

namespace pm_control
{
class ControlledProcessManager;
class Communication;
class PMControl : public QMainWindow
{
    Q_OBJECT

public:
    PMControl();
    virtual ~PMControl();
    const Communication* getComm();

    void sendProcessCommand(const essentials::Identifier* receiverId, std::vector<const essentials::Identifier*> robotIds, std::vector<int> execIds,
            std::vector<int> paramSets, int cmd);


    std::chrono::duration<double> msgTimeOut;

    Ui::PMControlWidget ui_;
    QWidget* widget_;

    process_manager::RobotExecutableRegistry* pmRegistry;

private:
    friend class Communication;
    Communication* comm;
    std::queue<std::pair<std::chrono::system_clock::time_point, process_manager::ProcessStats>> processStatMsgQueue;
    std::mutex msgQueueMutex;

    essentials::SystemConfig* sc;

    std::map<const essentials::Identifier*, pm_control::ControlledProcessManager*, essentials::IdentifierComparator> processManagersMap;

    void handleProcessStats();

    void receiveProcessStats(process_manager::ProcessStats psts);
    pm_control::ControlledProcessManager* getControlledProcessManager(const essentials::Identifier* processManagerId);

    QTimer* guiUpdateTimer;

public Q_SLOTS:
    void run();
    void updateGUI();
};

} // namespace pm_control
