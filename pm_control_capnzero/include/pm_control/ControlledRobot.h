#pragma once

#include <process_manager/containers/ProcessStat.h>
#include <process_manager/containers/ProcessStats.h>
#include <process_manager/containers/ProcessCommand.h>
#include <process_manager/RobotMetaData.h>

#include <QFrame>
#include <QHBoxLayout>
#include <QObject>
#include <chrono>

namespace Ui
{
class RobotProcessesWidget;
}

namespace essentials
{
class RobotExecutableRegistry;
class Identifier;
} // namespace  essentials
namespace pm_control {
    class Communication;
}

namespace pm_control
{
class ControlledExecutable;

class ControlledRobot : public QObject, public process_manager::RobotMetaData
{
    Q_OBJECT

public:
    ControlledRobot(std::string robotName, const essentials::Identifier* robotId, const essentials::Identifier* parentPMid, pm_control::Communication* comm); /*<for robot_control*/
    virtual ~ControlledRobot();

    void handleProcessStat(std::chrono::system_clock::time_point timeMsgReceived, process_manager::ProcessStat ps, const essentials::Identifier* parentPMid);
    void sendProcessCommand(process_manager::ProcessCommand pc);
    void updateGUI(std::chrono::system_clock::time_point now);
    void addExec(QWidget* exec);
    void removeExec(QWidget* exec);

    std::chrono::system_clock::time_point timeLastMsgReceived; /* < Time point, when the last message have been received */
    QFrame* robotProcessesQFrame;                              /**< The widget, used to initialise the RobotProcessesWidget */
    pm_control::Communication* comm;

  public Q_SLOTS:
    void updateBundles(QString text);

  private:
    std::chrono::duration<double> msgTimeOut;
    bool inRobotControl;
    std::string selectedBundle;
    Ui::RobotProcessesWidget* _robotProcessesWidget;
    std::map<int, ControlledExecutable*> controlledExecMap;
    const essentials::Identifier* parentPMid;
};

} /* namespace pm_widget */
