#pragma once

#include "pm_widget/ControlledRobot.h"

#include <essentials/Identifier.h>
#include <process_manager/containers/ProcessStat.h>
#include <process_manager/containers/ProcessStats.h>
#include <ui_PMControl.h>

#include <QDialog>
#include <QWidget>
#include <QtGui>
#include <ros/macros.h>
#include <ros/ros.h>
#include <rqt_gui_cpp/plugin.h>

#include <chrono>
#include <mutex>
#include <queue>
#include <utility>

namespace essentials
{
class SystemConfig;
} // namespace  essentials

namespace process_manager {
    class RobotExecutableRegistry;
}

namespace pm_widget
{
class ControlledProcessManager;
}

namespace pm_control
{
class PMControl : public rqt_gui_cpp::Plugin
{
    Q_OBJECT

  public:
    PMControl();
    virtual void initPlugin(qt_gui_cpp::PluginContext& context);
    virtual void shutdownPlugin();
    virtual void saveSettings(qt_gui_cpp::Settings& plugin_settings, qt_gui_cpp::Settings& instance_settings) const;
    virtual void restoreSettings(const qt_gui_cpp::Settings& plugin_settings, const qt_gui_cpp::Settings& instance_settings);

    void sendProcessCommand(const essentials::Identifier* receiverId, std::vector<const essentials::Identifier*> robotIds, std::vector<int> execIds,
            std::vector<int> paramSets, int cmd);

    std::chrono::duration<double> msgTimeOut;

    Ui::PMControlWidget ui_;
    QWidget* widget_;

    process_manager::RobotExecutableRegistry* pmRegistry;

  private:
    // TODO
//    ros::NodeHandle* rosNode;
//    ros::Subscriber processStateSub;
//    ros::Publisher processCommandPub;
    std::queue<std::pair<std::chrono::system_clock::time_point, process_manager::ProcessStats>> processStatMsgQueue;
    std::mutex msgQueueMutex;

    essentials::SystemConfig* sc;

    std::map<const essentials::Identifier*, pm_widget::ControlledProcessManager*, essentials::IdentifierComparator> processManagersMap;

    void handleProcessStats();

    void receiveProcessStats(process_manager::ProcessStats psts);
    pm_widget::ControlledProcessManager* getControlledProcessManager(const essentials::Identifier* processManagerId);

    QTimer* guiUpdateTimer;

  public Q_SLOTS:
    void run();
    void updateGUI();
};

} // namespace pm_control