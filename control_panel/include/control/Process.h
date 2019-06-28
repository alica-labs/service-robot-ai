#pragma once

#include <process_manager/ExecutableMetaData.h>
#include <process_manager/containers/ProcessStat.h>
#include <qt5/QtWidgets/QWidget>

#include <chrono>
#include <map>
#include <string>
#include <vector>

namespace Ui
{
    class Process;
    class Agent;
} // namespace Ui

namespace process_manager
{
    class ExecutableMetaData;
    class RobotExecutableRegistry;
} // namespace  essentials

namespace control
{
    class ExecutableRegistry;
    class Agent;
    class Process : public QObject
    {
    Q_OBJECT

    public:
        Process(Agent* parentAgent, int processKey, ExecutableRegistry* executableRegistry);
        virtual ~Process();

        void update(std::chrono::system_clock::time_point timeMsgReceived, process_manager::ProcessStat ps);
        void update(std::chrono::system_clock::time_point now);
        void handleBundleComboBoxChanged(QString bundle);
        void sendProcessCommand(int cmd);

        std::chrono::duration<double> msgTimeOut;
        std::chrono::system_clock::time_point timeLastMsgReceived; /**< last time a message was received for this executable */

        const process_manager::ExecutableMetaData* execMetaData;
        int runningParamSet;
        int desiredParamSet;
        char state; /**< The process state (zombie, running, etc.) */
        unsigned short cpu;
        long int memory;
        bool publishing;

        QWidget* processWidget;
        Ui::Process* uiProcess;
        ExecutableRegistry* executableRegistry;

    public Q_SLOTS:
        void handleCheckBoxStateChanged(int newState);
        void showContextMenu(const QPoint& pos);

    Q_SIGNALS:
        void processCheckBoxStateChanged(int, int); /**< first int is newState, second int is execId */

    private:
        static const std::string redBackground;
        static const std::string greenBackground;
        static const std::string grayBackground;
        Agent* parentAgent;
    };

} /* namespace pm_widget */

