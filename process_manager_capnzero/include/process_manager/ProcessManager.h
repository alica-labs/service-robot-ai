#pragma once

#define PM_DEBUG // for toggling debug output

#include "process_manager/containers/ProcessCommand.h"

#include <essentials/SystemConfig.h>
#include <essentials/Identifier.h>

#include <chrono>

namespace std
{
class thread;
}

namespace process_manager
{

class ManagedRobot;
class ManagedExecutable;
class RobotMetaData;
class ExecutableMetaData;
class RobotExecutableRegistry;
class Communication;
class ProcessManager
{
public:
    ProcessManager(int argc, char** argv);
    virtual ~ProcessManager();
    void start();
    bool isRunning();

    bool selfCheck();
    void initCommunication(int argc, char** argv);
    bool isKnownInterpreter(std::string const& cmdLinePart);
    std::vector<std::string> splitCmdLine(std::string cmdLine);

    RobotExecutableRegistry* getPMRegistry() { return this->pmRegistry; };
    const essentials::Identifier* getOwnID() { return this->ownId; };
    bool getSimMode() { return this->simMode; };

    static void pmSigintHandler(int sig);
    static void pmSigchildHandler(int sig);
    static std::string getCmdLine(const char* pid);
    static int numCPUs;  /* < including hyper threading cores */
    static bool running; /* < has to be static, to be changeable within ProcessManager::pmSignintHandler() */

private:
    friend Communication;
    essentials::SystemConfig& sc;
    Communication* communication;
    std::string ownHostname;
    const essentials::Identifier* ownId;
    bool simMode;
    std::map<const essentials::Identifier*, ManagedRobot*, essentials::IdentifierComparator> robotMap;
    RobotExecutableRegistry* pmRegistry;
    std::vector<std::string> interpreters;
    unsigned long long lastTotalCPUTime;
    unsigned long long currentTotalCPUTime;

    std::string getRobotEnvironmentVariable(std::string processId);
    void updateTotalCPUTimes();
    void changeDesiredProcessStates(ProcessCommand pc, bool shouldRun);
    void changeLogPublishing(ProcessCommand pc, bool shouldPublish);
    std::thread* mainThread;
    std::chrono::microseconds iterationTime;

    void run();
    void searchProcFS();
    void update(unsigned long long cpuDelta);
    void report();
};

} // namespace  essentials
