#include "control/ExecutableRegistry.h"

#include "process_manager/ExecutableMetaData.h"
#include "process_manager/RobotMetaData.h"

#include <ConsoleCommandHelper.h>

#include <iostream>
#include <string.h>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::map;
using std::pair;
using std::runtime_error;
using std::string;
using std::stringstream;
using std::vector;

namespace control
{

ExecutableRegistry* ExecutableRegistry::get()
{
    static ExecutableRegistry instance;
    return &instance;
}

ExecutableRegistry::ExecutableRegistry()
        : sc(essentials::SystemConfig::getInstance())
{
}

ExecutableRegistry::~ExecutableRegistry()
{
    for (auto metaData : this->executableList) {
        delete metaData;
    }
}

const map<string, vector<pair<int, int>>>* const ExecutableRegistry::getBundlesMap()
{
    if (bundlesMap.size() == 0) {
        // Read bundles from Processes.conf
        auto bundlesSections = (*this->sc)["ProcessManaging"]->getSections("Processes.Bundles", NULL);
        for (auto bundleName : (*bundlesSections)) {
            vector<int> processList = (*this->sc)["ProcessManaging"]->getList<int>("Processes.Bundles", bundleName.c_str(), "processList", NULL);
            vector<string> processParamsList =
                    (*this->sc)["ProcessManaging"]->getList<string>("Processes.Bundles", bundleName.c_str(), "processParamsList", NULL);
            if (processList.size() != processParamsList.size()) {
                cerr << "RobotExecutableReg: Number of processes does not match the number of parameter sets for the "
                        "bundle '"
                     << bundleName << "' in the ProcessManaging.conf!" << endl;
                continue;
            }

            for (int i = 0; i < processList.size(); i++) {
                this->bundlesMap[bundleName].push_back(pair<int, int>(processList[i], stoi(processParamsList[i])));
            }
            cout << "RobotExecutableReg: Bundle '" << bundleName << "' has " << this->bundlesMap[bundleName].size() << " processes." << endl;
        }
    }
    return &bundlesMap;
}

bool ExecutableRegistry::getExecutableName(int execId, string& execName)
{
    for (auto execMetaData : this->executableList) {
        if (execMetaData->id == execId) {
            execName = execMetaData->name;
            return true;
        }
    }

    execName = "";
    return false;
}

bool ExecutableRegistry::getExecutableIdByExecName(string execName, int& execId)
{
    for (auto execMetaData : this->executableList) {
        if (execMetaData->execName == execName) {
            execId = execMetaData->id;
            return true;
        }
    }

    execId = 0;
    return false;
}

bool ExecutableRegistry::getExecutableId(vector<string>& splittedCmdLine, int& execId)
{
    for (auto execMetaData : this->executableList) {
        if (execMetaData->matchSplittedCmdLine(splittedCmdLine)) {
            execId = execMetaData->id;
            return true;
        }
    }
    execId = 0;
    return false;
}

bool ExecutableRegistry::executableExists(int execId)
{
    for (auto execMetaData : this->executableList) {
        if (execMetaData->id == execId) {
            return true;
        }
    }

    return false;
}

bool ExecutableRegistry::executableExists(string execName)
{
    for (auto execMetaData : this->executableList) {
        if (execMetaData->name == execName) {
            return true;
        }
    }

    return false;
}

/**
 * This method registers the given executable, if it is listed in the ProcessManaging.conf file.
 * @param execName
 * @return -1, if the executable is not registered, due to some error. Otherwise, it returns the registered id.
 */
int ExecutableRegistry::addExecutable(string execSectionName)
{
    if (this->executableExists(execSectionName)) {
        cerr << "RobotExecutableRegistry: The executable '" << execSectionName << "' is already registered!" << endl;
        return -1;
    }

    essentials::SystemConfig* sc = essentials::SystemConfig::getInstance();
    int execId;
    string processMode;
    string execName;
    string absExecName;
    string rosPackage = "NOT-FOUND"; // optional
    string prefixCmd = "NOT-FOUND";  // optional

    try {
        execId = (*sc)["ProcessManaging"]->get<int>("Processes.ProcessDescriptions", execSectionName.c_str(), "id", NULL);
        processMode = (*sc)["ProcessManaging"]->get<string>("Processes.ProcessDescriptions", execSectionName.c_str(), "mode", NULL);
        execName = (*sc)["ProcessManaging"]->get<string>("Processes.ProcessDescriptions", execSectionName.c_str(), "execName", NULL);
        rosPackage = (*sc)["ProcessManaging"]->tryGet<string>("NOT-FOUND", "Processes.ProcessDescriptions", execSectionName.c_str(), "rosPackage", NULL);
        prefixCmd = (*sc)["ProcessManaging"]->tryGet<string>("NOT-FOUND", "Processes.ProcessDescriptions", execSectionName.c_str(), "prefixCmd", NULL);
    } catch (runtime_error& e) {
        cerr << "PM-Registry: Cannot add executable '" << execSectionName << "', because of faulty values in ProcessManaging.conf!" << endl;
        return -1;
    }

    // create absolute executable name, if possible
    if (rosPackage.compare("NOT-FOUND") != 0 && prefixCmd.compare("roslaunch") != 0) {
        string cmd = "catkin_find --first-only --libexec " + rosPackage;
        absExecName = essentials::ConsoleCommandHelper::exec(cmd.c_str());

        if (absExecName.length() > 1) {
            absExecName = absExecName.substr(0, absExecName.length() - 1);
            absExecName = absExecName + "/" + execName;
        }
    }

    process_manager::ExecutableMetaData* execMetaData =
            new process_manager::ExecutableMetaData(execSectionName, execId, processMode, execName, rosPackage, prefixCmd, absExecName);
    auto paramSets = (*sc)["ProcessManaging"]->tryGetNames("NONE", "Processes.ProcessDescriptions", execSectionName.c_str(), "paramSets", NULL);
    if (paramSets->size() > 1 || paramSets->at(0) != "NONE") {
        for (string paramSetKeyString : (*paramSets)) {
            try {
                int paramSetKey = stoi(paramSetKeyString);
                auto paramSetValues = (*sc)["ProcessManaging"]->getList<string>(
                        "Processes.ProcessDescriptions", execSectionName.c_str(), "paramSets", paramSetKeyString.c_str(), NULL);

                // first param is always the executable name
                vector<char*> currentParams;
                if (absExecName.length() > 1) {
                    currentParams.push_back(strdup(absExecName.c_str()));
                } else {
                    currentParams.push_back(strdup(execName.c_str()));
                }
                // transform the system config params to vector of char*, for c-compatibility.
                cout << currentParams[0] << endl;
                for (string param : paramSetValues) {
                    char* tmp = new char[param.size() + 1];
                    strcpy(tmp, param.c_str());
                    tmp[param.size()] = '\0';
                    currentParams.push_back(tmp);
                }
                currentParams.push_back(nullptr);

                execMetaData->addParameterSet(paramSetKey, currentParams);
            } catch (exception& e) {
                cerr << "RobotExecutableRegistry: Unable to parse parameter set \"" << paramSetKeyString << "\" of process \"" << execSectionName << "\""
                     << endl;
                cerr << e.what() << endl;
            }
        }
    } else {
        vector<char*> currentParams;
        if (absExecName.length() > 1) {
            currentParams.push_back(strdup(absExecName.c_str()));
        } else {
            currentParams.push_back(strdup(execName.c_str()));
        }
        currentParams.push_back(nullptr);
        execMetaData->addParameterSet(0, currentParams);
    }

    // cout << (*execMetaData) << endl;
    this->executableList.push_back(execMetaData);
    return execId;
}

/**
 * This method is for copying meta data from an entry into a real managed executable.
 * Don't change anything in the returns object.
 * @param execName is the name of the demanded entry.
 * @return The demanded entry, if it exists. nullptr, otherwise.
 */
process_manager::ExecutableMetaData const* const ExecutableRegistry::getExecutable(string execName) const
{
    for (auto execEntry : this->executableList) {
        if (execEntry->name == execName) {
            return execEntry;
        }
    }
    return nullptr;
}

/**
 * This method is for copying meta data from an entry into a real managed executable.
 * Don't change anything in the returns object.
 * @param execId is the id of the demanded entry.
 * @return The demanded entry, if it exists. nullptr, otherwise.
 */
process_manager::ExecutableMetaData const* const ExecutableRegistry::getExecutable(int execId) const
{
    for (auto execEntry : this->executableList) {
        if (execEntry->id == execId) {
            return execEntry;
        }
    }
    return nullptr;
}

/**
 * For accessing the internal data structure of executable meta data entries.
 * @return The internal data structure of executable meta data entries.
 */
const vector<process_manager::ExecutableMetaData*>& ExecutableRegistry::getExecutables() const
{
    return this->executableList;
}

void ExecutableRegistry::setInterpreters(vector<string> interpreter)
{
    this->interpreter = interpreter;
}

} // namespace control
