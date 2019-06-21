#pragma once

#define MNG_ROBOT_DEBUG

#include "process_manager/containers/ProcessStat.h"
#include "process_manager/containers/ProcessStats.h"
#include "process_manager/RobotExecutableRegistry.h"
#include "process_manager/RobotMetaData.h"

#include <map>
#include <string>
#include <vector>

namespace process_manager
{
class ManagedExecutable;
class ProcessManager;

class ManagedRobot : public RobotMetaData
{
public:
    ManagedRobot(std::string robotName, const essentials::Identifier* agentID, ProcessManager* procMan);
    virtual ~ManagedRobot();
    void queue4update(int execid, long pid, RobotExecutableRegistry* registry);
    void update(unsigned long long cpuDelta);
    void startExecutable(std::string execName, int execid);
    void changeDesiredState(int execId, int paramSetId, bool shouldRun, RobotExecutableRegistry* registry);
    void changeDesiredState(int execId, bool shouldRun, RobotExecutableRegistry* registry);
    void changeLogPublishing(int execId, bool shouldPublish, RobotExecutableRegistry* registry);
    void report(process_manager::ProcessStats& psts);

  private:
    ProcessManager* procMan;
    std::map<int, ManagedExecutable*> executableMap;
};

} /* namespace  essentials */
