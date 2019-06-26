#include "process_manager/ProcessManager.h"

#include "process_manager/ManagedExecutable.h"
#include "process_manager/ManagedRobot.h"
#include "process_manager/RobotExecutableRegistry.h"
#include "process_manager/Communication.h"
#include "process_manager/containers/ProcessStats.h"

#include <Logging.h>

#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <map>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

namespace process_manager
{

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::thread;
using std::vector;

bool ProcessManager::running = false;
int ProcessManager::numCPUs = 0;
/**
 * Creates an ProcessManager object, which has already parsed the process descriptions of the processes to be managed
 * and the robots, which are known for the ROBOT environment variable.
 * @param argc
 * @param argv
 */
ProcessManager::ProcessManager(int argc, char** argv)
        : iterationTime(1000000)
        , mainThread(NULL)
        , lastTotalCPUTime(0)
        , currentTotalCPUTime(0)
        , simMode(false)
        , ownId(nullptr)
{
    this->sc = essentials::SystemConfig::getInstance();
    this->ownHostname = this->sc->getHostname();
    this->pmRegistry = RobotExecutableRegistry::get();

    /* Initialise some data structures for better performance in searchProcFS-Method with
     * data from Globals.conf and Processes.conf file. */

    // Register robots from Globals.conf
    const essentials::Identifier* tmpAgentID;
    auto robotNames = (*this->sc)["Globals"]->getSections("Globals.Team", NULL);
    for (auto robotName : (*robotNames)) {
        tmpAgentID = this->pmRegistry->addRobot(robotName);
        if (robotName == this->ownHostname) {
            this->ownId = tmpAgentID;
        }
    }

    // Register local hostname, if it does not exist in Globals.conf
    if (this->ownId == nullptr) {
        this->ownId = this->pmRegistry->addRobot(this->ownHostname);
    }

    // autostart for robots
    bool autostart = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-autostart") == 0) {
            autostart = true;
            break;
        } else if (strcmp(argv[i], "-sim") == 0) {
            this->simMode = true;
        }
    }

    if (autostart) {
        // Create ManagedRobot-Object for local system/robot
        this->robotMap.emplace(ownId, new ManagedRobot(this->ownHostname, ownId, this));
    }

    // Register executables from Processes.conf
    int tmpExecID = -1;
    auto processDescriptions = (*this->sc)["ProcessManaging"]->getSections("Processes.ProcessDescriptions", NULL);
    for (auto processSectionName : (*processDescriptions)) {
        tmpExecID = this->pmRegistry->addExecutable(processSectionName);
        // This autostart functionality is for easier testing. The local robot starts the processes automatically
        if (autostart && tmpExecID != -1 && this->pmRegistry->getExecutable(tmpExecID)->mode == "autostart") {
            this->robotMap.at(ownId)->changeDesiredState(tmpExecID, true, this->pmRegistry);
        }
    }

    this->interpreters = (*this->sc)["ProcessManaging"]->getList<string>("Processes.Interpreter", NULL);
    cout << "PM: Number of Interpreters: " << this->interpreters.size() << endl;
    this->pmRegistry->setInterpreters(interpreters);

    this->communication = new Communication(this);

    cout << "PM: OwnId is " << *ownId << endl;
}

ProcessManager::~ProcessManager()
{
    ProcessManager::running = false;
    if (this->mainThread != nullptr) {
        mainThread->join();
        delete mainThread;
    }

    for (auto mngdRobot : this->robotMap) {
        delete mngdRobot.second;
    }
    this->robotMap.clear();
    delete this->communication;
}

void ProcessManager::changeLogPublishing(process_manager::ProcessCommand pc, bool shouldPublish)
{
    for (const auto& agentIDros : pc.robotIDs) {
        // Check whether the robot with the given id is known
        std::string robotName;
        if (const essentials::Identifier* agentID = this->pmRegistry->getRobotId(robotName, agentIDros->getRaw(), agentIDros->getSize())) {
            // Find the ManagedRobot object
            auto mapIter = this->robotMap.find(agentID);
            ManagedRobot* mngdRobot;
            if (mapIter == this->robotMap.end()) {
                // Lazy initialisation of the robotMap
                mngdRobot = this->robotMap.emplace(agentID, new ManagedRobot(robotName, agentID, this)).first->second;
            } else {
                // ManagedRobot already exists
                mngdRobot = mapIter->second;
            }

            for (int i = 0; i < pc.processKeys.size(); i++) {
                mngdRobot->changeLogPublishing(pc.processKeys[i], shouldPublish, this->pmRegistry);
            }
        } else {
            cout << "PM: Received command for unknown robot id: " << agentID << endl;
        }
    }
}

/**
 * This method processes an incoming ROS-Message, received in ::handleProcessCommand.
 * @param pc
 * @param shouldRun
 */
void ProcessManager::changeDesiredProcessStates(process_manager::ProcessCommand pc, bool shouldRun)
{
    if (pc.processKeys.size() != pc.paramSets.size()) {
        cerr << "PM: Received malformed process command! #ProcessKeys != #ParamSets" << endl;
        return;
    }

    for (const auto& agentIDros : pc.robotIDs) {
        // Check whether the robot with the given id is known
        std::string robotName;
        if (const essentials::Identifier* agentID = this->pmRegistry->getRobotId(robotName, agentIDros->getRaw(), agentIDros->getSize())) {
            // Find the ManagedRobot object
            auto mapIter = this->robotMap.find(agentID);
            ManagedRobot* mngdRobot;
            if (mapIter == this->robotMap.end()) {
                // Lazy initialisation of the robotMap
                mngdRobot = this->robotMap.emplace(agentID, new ManagedRobot(robotName, agentID, this)).first->second;
            } else {
                // ManagedRobot already exists
                mngdRobot = mapIter->second;
            }

            for (int i = 0; i < pc.processKeys.size(); i++) {
                mngdRobot->changeDesiredState(pc.processKeys[i], pc.paramSets[i], shouldRun, this->pmRegistry);
            }
        } else {
            cout << "PM: Received command for unknown robot id: " << agentID << endl;
        }
    }
}

/**
 * Starts the worker thread of the ProcessManager, if not already running.
 */
void ProcessManager::start()
{
    if (!ProcessManager::running) {
        ProcessManager::running = true;
        this->mainThread = new thread(&ProcessManager::run, this);
    }
}

/**
 * The run-method of the worker thread of the ProcessManager object.
 */
void ProcessManager::run()
{
    while (ProcessManager::running) {
        auto start = std::chrono::system_clock::now();

        this->searchProcFS();
        this->updateTotalCPUTimes();
        this->update((this->currentTotalCPUTime - this->lastTotalCPUTime) / ProcessManager::numCPUs);
        this->report();

        auto timePassed = std::chrono::system_clock::now() - start;
        std::chrono::microseconds microsecondsPassed = std::chrono::duration_cast<std::chrono::microseconds>(timePassed);

#ifdef PM_DEBUG
        cout << "PM: " << microsecondsPassed.count() << " microseconds passed!" << endl << endl;
#endif
        std::chrono::microseconds availTime = this->iterationTime - microsecondsPassed;

        if (availTime.count() > 10) {
            std::this_thread::sleep_for(availTime);
        }
    }
}

/**
 * Updates lastTotalCPUTime and currentTotalCPUTime by
 * summing up the first line of /proc/stat.
 */
void ProcessManager::updateTotalCPUTimes()
{
    this->lastTotalCPUTime = currentTotalCPUTime;

    string statLine;
    std::ifstream statFile("/proc/stat", std::ifstream::in);
    getline(statFile, statLine);
    statFile.close();

    statLine = statLine.substr(4, statLine.length() - 4);
    // cout << "PM: readTotalCPUTime() statline: " << statLine << endl;

    long tmp;
    stringstream statStream(statLine);

    int i = 0;
    this->currentTotalCPUTime = 0;
    while (statStream.good() && i < 10) {
        statStream >> tmp;
        this->currentTotalCPUTime += tmp;
    }

    /*cout << "PM: Last total CPU time is " << this->lastTotalCPUTime << endl;
     cout << "PM: Current total CPU time is " << this->currentTotalCPUTime << endl;*/
}

/**
 * Publishes a ProcessStats-Message.
 */
void ProcessManager::report()
{
    process_manager::ProcessStats psts;
    psts.senderID = this->ownId;
    for (auto const& mngdRobot : this->robotMap) {
        // cout << "PM: report() We try to add another ProcessStat from " << mngdRobot.second->name << "!" << endl;
        mngdRobot.second->report(psts);
    }
     cout << "PM: report() We have " << psts.processStats.size() << " ProcessStats!" << endl;
    this->communication->sendProcessStats(psts);
}

/**
 * Calls update on all ManagedRobot instances.
 */
void ProcessManager::update(unsigned long long cpuDelta)
{
    for (auto const& mngdRobot : this->robotMap) {
        mngdRobot.second->update(cpuDelta);
    }
}

/**
 * Searches the proc filesystem for instances of processes, the ProcessManager has to manage.
 */
void ProcessManager::searchProcFS()
{
    DIR* proc;
    struct dirent* dirEntry;

    if (!(proc = opendir("/proc"))) {
        perror("PM: can't open /proc");
        return;
    }

    long curPID = 0;
    char* endPtr;
    string curExecutable;
    while ((dirEntry = readdir(proc)) != NULL) {
        /* if endptr is not a null character, the directory is not
         * entirely numeric, so ignore it */
        curPID = strtol(dirEntry->d_name, &endPtr, 10);
        if (*endPtr != '\0') {
            continue;
        }

        // get the cmdline
        string cmdLine = this->getCmdLine(dirEntry->d_name);

        // ignore "kernel processes"
        if (cmdLine.length() == 0)
            continue;

        vector<string> splittedCmdLine = this->splitCmdLine(cmdLine);

        // ignore interpreter
        if (isKnownInterpreter(splittedCmdLine[0])) {
            splittedCmdLine.erase(splittedCmdLine.begin());
        }

        int execId;
        if (this->pmRegistry->getExecutableId(splittedCmdLine, execId)) {
            // get the robots name from the ROBOT environment variable
            string robotName = this->getRobotEnvironmentVariable(string(dirEntry->d_name));
            const essentials::Identifier* agentID = this->pmRegistry->getRobotId(robotName);
            if (!agentID) {
                cout << "PM: Warning! Unknown robot '" << robotName << "' is running executable with ID '" << execId << "'" << endl;
                agentID = this->pmRegistry->addRobot(robotName);
            }

            auto robotEntry = this->robotMap.find(agentID);
            if (robotEntry != robotMap.end()) {
                robotEntry->second->queue4update(execId, curPID, this->pmRegistry);
            } else {
                this->robotMap.emplace(agentID, new ManagedRobot(robotName, agentID, this)).first->second->queue4update(execId, curPID, this->pmRegistry);
            }

#ifdef PM_DEBUG
            cout << "PM: Robot '" << robotName << "' (ID:" << *agentID << ") executes executable with ID '" << execId << "' with PID " << curPID << endl;
#endif
        }
        // else: continue, as this executable is unknown and not to be managed by the process manager
    }

    closedir(proc);
}

/**
 * Reads the command line entry in the proc-filesystem for the given process ID.
 * @param pid
 * @return The executable name as string.
 */
string ProcessManager::getCmdLine(const char* pid)
{
    string cmdline;
    std::ifstream cmdlineStream("/proc/" + string(pid) + "/cmdline", std::ifstream::in);
    getline(cmdlineStream, cmdline);

    //		cout << "getCmdLine: ";
    //		while (!getline(cmdlineStream, cmdline, '\0').eof())
    //		{
    //			 cout << "'" << cmdline << "' ";
    //		}
    //		cout << "\n" << endl;
    cmdlineStream.close();
    return cmdline;
}

/**
 * Get the robots name from the ROBOT environment variable of the given process.
 * @param processId
 * @return The value of the ROBOT environment variable, if present. Local hostname, otherwise.
 */
string ProcessManager::getRobotEnvironmentVariable(string processId)
{
    string curFile = "/proc/" + processId + "/environ";
    std::ifstream ifs(curFile, std::ifstream::in);
    string robotEnvironment;

    while (!getline(ifs, robotEnvironment, '\0').eof()) {
        if (robotEnvironment.substr(0, 6).compare("ROBOT=") == 0) {
            return robotEnvironment.substr(6, 256);
        }
    }

    ifs.close();
    return this->ownHostname;
}

/**
 * Method for checking, whether the ProcessManager's main thread is still running.
 * @return running
 */
bool ProcessManager::isRunning()
{
    return running;
}

/**
 * Checks whether another instance of the process manager executable is already
 * running on the system and set the kernelPageSize of ManagedExecutable. Furthermore,
 * this methods starts a roscore if necessary.
 * @return False, if something didn't work out. True, otherwise.
 */
bool ProcessManager::selfCheck()
{
    string roscoreExecName = "roscore";
    std::ifstream ifs("/proc/self/stat", std::ifstream::in);
    string pid;
    getline(ifs, pid, '\0');
    ifs.close();
    long ownPID = stol(pid);

    DIR* proc;
    struct dirent* dirEntry;

    if (!(proc = opendir("/proc"))) {
        perror("PM: can't open /proc");
        return true;
    }

    long curPID = 0;
    char* endPtr;
    string curExecutable;
    while ((dirEntry = readdir(proc)) != NULL) {
        /* if endptr is not a null character, the directory is not
         * entirely numeric, so ignore it */
        curPID = strtol(dirEntry->d_name, &endPtr, 10);
        if (*endPtr != '\0') {
            continue;
        }

        // get the executables name
        string cmdLine = getCmdLine(dirEntry->d_name);
        if (cmdLine.length() == 0)
            continue;

        // Check for already running process managers
        vector<string> splittedCmdLine = splitCmdLine(cmdLine);
        if (ownPID != curPID && cmdLine.find("gdb") == string::npos) {
            for (string cmdLinePart : splittedCmdLine) {
                // cout << cmdLinePart << " " << cmdLinePart.find_last_of("process_manager") << " " <<
                // cmdLinePart.length()-1 << endl;
                if (cmdLinePart.length() >= 15 && cmdLinePart.compare(cmdLinePart.length() - 15, 15, "process_manager") == 0) {
                    cerr << "PM: My own PID is " << ownPID << endl;
                    cerr << "PM: There is already another process_manager running on this system! PID: " << curPID << endl;
                    cerr << "PM: Terminating myself..." << endl;
                    closedir(proc);
                    return false;
                }
            }
        }
    }

    closedir(proc);
    return true;
}

/**
 * Checks whether the found executable name is an interpreter like python, java, or ruby.
 * @param execName
 * @return True, if it is a known interpreter. False, otherwise.
 */
bool ProcessManager::isKnownInterpreter(string const& cmdLinePart)
{
    int lastSlashIdx = cmdLinePart.find_last_of('/');
    return find(this->interpreters.begin(), this->interpreters.end(), cmdLinePart.substr(lastSlashIdx + 1, cmdLinePart.length())) != this->interpreters.end();
}

/**
 * Splits the given command line at '\0' characters.
 */
vector<string> ProcessManager::splitCmdLine(string cmdLine)
{
    vector<string> splittedCmdLine;
    int argIdx = 0;
    int argEndPos = cmdLine.find('\0', argIdx);

    if (argEndPos == string::npos) // catch the case for only one argument
    {
        splittedCmdLine.push_back(cmdLine);
        return splittedCmdLine;
    }

    while (argEndPos != string::npos) {
        splittedCmdLine.push_back(cmdLine.substr(argIdx, argEndPos - argIdx));
        argIdx = argEndPos + 1;
        argEndPos = cmdLine.find('\0', argIdx);
    }
    return splittedCmdLine;
}

/**
 * This is for handling Strg + C, although no ROS communication was running.
 * @param sig
 */
void ProcessManager::pmSigintHandler(int sig)
{
    cout << endl << "PM: Caught SIGINT! Terminating ..." << endl;
    running = false;

    // TODO: See if you need to shutdown some capnzero stuff
}

/**
 *
 * @param sig
 */
void ProcessManager::pmSigchildHandler(int sig)
{
    /* Wait for all dead processes.
     * We use a non-blocking call to be sure this signal handler will not
     * block if a child was cleaned up in another part of the program. */
    int result = 0;
    do {
        result = waitpid(WAIT_ANY, NULL, WNOHANG);
        if (result > 0) {
            cout << "PM: Caught SIGCHLD Signal for PID: " << result << endl;
        } else if (result < 0) {
            /* NOTE: I think, that we always get ECHILD, when a child process dies, because
             * we had to set ssid of the child processes, in order to let them live if the process
             * manager dies. Anyway, it is not that dramatic.
             */
            if (errno != ECHILD) {
                cerr << "PM: 'waitpid' returned an error! " << endl;
                perror("waitpid");
            }
        }

    } while (result > 0);
}

} /* namespace  essentials */

int main(int argc, char** argv)
{
    // Set kernel page size for human readable memory consumption
    process_manager::ManagedExecutable::kernelPageSize = sysconf(_SC_PAGESIZE);
    // Determine number of cores
    while (essentials::FileSystem::pathExists("/sys/devices/system/cpu/cpu" + std::to_string(process_manager::ProcessManager::numCPUs))) {
        process_manager::ProcessManager::numCPUs++;
    }

    process_manager::ProcessManager* pm = new process_manager::ProcessManager(argc, argv);
    signal(SIGINT, process_manager::ProcessManager::pmSigintHandler);
    signal(SIGCHLD, process_manager::ProcessManager::pmSigchildHandler);

    if (pm->selfCheck()) {
        pm->start();
        while (pm->isRunning()) {
            std::chrono::milliseconds dura(500);
            std::this_thread::sleep_for(dura);
        }
        delete pm;
    }

    return 0;
}
