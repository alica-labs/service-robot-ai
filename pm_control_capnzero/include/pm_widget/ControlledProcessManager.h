#include <process_manager/containers/ProcessCommand.h>
#include <process_manager/containers/ProcessStats.h>

#include <QBoxLayout>
#include <QFrame>
#include <chrono>
#include <ros/ros.h>
#include <string>
#include <utility>

#include <essentials/Identifier.h>

namespace essentials
{
class SystemConfig;
} // namespace  essentials

namespace process_manager {
    class RobotExecutableRegistry;
}

namespace pm_control
{
class PMControl;
}

namespace pm_widget
{
class ControlledRobot;

class ControlledProcessManager : public QObject
{
    Q_OBJECT

public:
    ControlledProcessManager(std::string processManagerName, const essentials::Identifier* processManagerId, QBoxLayout* pmHorizontalLayout);
    virtual ~ControlledProcessManager();

    void updateGUI(std::chrono::system_clock::time_point now);
    void handleProcessStats(std::pair<std::chrono::system_clock::time_point, process_manager::ProcessStats> timePstsPair);
    void addRobot(QFrame* robot);
    void removeRobot(QFrame* robot);

    void hide();
    void show();

    std::chrono::duration<double> msgTimeOut;
    std::chrono::system_clock::time_point timeLastMsgReceived; /* < Time point, when the last message have been received */
    std::string name;                                          /* < Hostname under which this process manager is running */
    const essentials::Identifier* id;                             /* < The id of the host */
    process_manager::RobotExecutableRegistry* pmRegistry;

private:
    std::map<const essentials::Identifier*, ControlledRobot*, essentials::IdentifierComparator>
            controlledRobotsMap; /* < The robots, which are controlled by this process manager */
    QBoxLayout* parentLayout;
    ControlledRobot* getControlledRobot(const essentials::Identifier* robotId);
};

} /* namespace pm_widget */
