#pragma once

#include <essentials/IdentifierConstPtr.h>
#include <process_manager/containers/ProcessStats.h>
#include <engine/containers/AlicaEngineInfo.h>

#include <QMainWindow>
#include <QtGui/QtGui>

#include <map>
#include <queue>
#include <mutex>

namespace essentials {
    class IDManager;
}

namespace control
{
class ExecutableRegistry;
class Agent;
class Communication;
class ControlPanel : public QMainWindow
{
    Q_OBJECT
public:
    ControlPanel();
    virtual ~ControlPanel();

    ExecutableRegistry* getExecutableRegistry();
    essentials::IDManager* getIDManager();
    void enqueue(process_manager::ProcessStats psts);
    void enqueue(alica::AlicaEngineInfo aei);

    QWidget* rootWidget_;

public Q_SLOTS:
    void run();

private:
    void processMessage();
    void updateUI();
    Agent* getAgent(essentials::IdentifierConstPtr id);

    QTimer* doWorkTimer;

    Communication* comm;
    std::mutex queueMutex;
    std::queue<std::pair<std::chrono::system_clock::time_point, process_manager::ProcessStats>> processStatsQueue;
    std::queue<std::pair<std::chrono::system_clock::time_point, alica::AlicaEngineInfo>> alicaInfosQueue;

    essentials::IDManager* idManager;
    ExecutableRegistry* executableRegistry;
    std::map<essentials::IdentifierConstPtr, Agent*> agents;
};

} // namespace control