#pragma once 

#include <process_manager/containers/ProcessStat.h>
#include <qt5/QtWidgets/QWidget>

#include <chrono>
#include <map>
#include <string>
#include <vector>

namespace Ui
{
class ProcessWidget;
class RobotProcessesWidget;
} // namespace Ui

namespace process_manager
{
class ExecutableMetaData;
class RobotExecutableRegistry;
} // namespace  essentials

using namespace std;

namespace pm_widget
{
class ControlledRobot;

class ControlledExecutable : public QObject
{
    Q_OBJECT

public:
    ControlledExecutable(process_manager::ExecutableMetaData* metaExec, ControlledRobot* parentRobot);
    virtual ~ControlledExecutable();

    void handleStat(chrono::system_clock::time_point timeMsgReceived, process_manager::ProcessStat ps);
    void updateGUI(chrono::system_clock::time_point now);
    void handleBundleComboBoxChanged(QString bundle);
    void sendProcessCommand(int cmd);

    chrono::duration<double> msgTimeOut;
    chrono::system_clock::time_point timeLastMsgReceived; /**< last time a message was received for this executable */

    int runningParamSet;
    int desiredParamSet;
    char state; /**< The process state (zombie, running, etc.) */
    unsigned short cpu;
    long int memory;
    bool publishing;

    process_manager::ExecutableMetaData* metaExec;
    QWidget* processWidget;
    Ui::ProcessWidget* _processWidget;

  public Q_SLOTS:
    void handleCheckBoxStateChanged(int newState);
    void showContextMenu(const QPoint& pos);

  Q_SIGNALS:
    void processCheckBoxStateChanged(int, int); /**< first int is newState, second int is execId */

  private:
    static const string redBackground;
    static const string greenBackground;
    static const string grayBackground;
    ControlledRobot* parentRobot;
    process_manager::RobotExecutableRegistry* pmRegistry;
};

} /* namespace pm_widget */

