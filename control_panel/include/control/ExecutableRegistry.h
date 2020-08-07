#pragma once

#include <essentials/SystemConfig.h>

#include <map>
#include <stdint.h>
#include <string>
#include <vector>

namespace process_manager
{
class ExecutableMetaData;
}

namespace control
{
/**
 * The RobotExecutableRegistry help the process manager and its
 * control GUI to remember/manage the names and IDs of robots and executables.
 */
class ExecutableRegistry
{
public:
    static ExecutableRegistry* get();

    void setInterpreters(std::vector<std::string> interpreter);
    bool isKnownInterpreter(std::string const& cmdLinePart);
    std::map<std::string, std::vector<std::pair<int, int>>> const* const getBundlesMap();
    process_manager::ExecutableMetaData const* const getExecutable(std::string execName) const;
    process_manager::ExecutableMetaData const* const getExecutable(int execId) const;
    const std::vector<process_manager::ExecutableMetaData*>& getExecutables() const;
    int addExecutable(std::string execName);
    bool getExecutableId(std::vector<std::string>& splittedCmdLine, int& execId);
    bool getExecutableIdByExecName(std::string execName, int& execId);
    bool getExecutableName(int execId, std::string& execName);
    bool executableExists(int execId);
    bool executableExists(std::string execName);

private:
    ExecutableRegistry();
    virtual ~ExecutableRegistry();

    essentials::SystemConfig& sc;
    std::vector<process_manager::ExecutableMetaData*> executableList;
    std::vector<std::string> interpreter;
    std::map<std::string, std::vector<std::pair<int, int>>> bundlesMap;
};

} /* namespace  control */
