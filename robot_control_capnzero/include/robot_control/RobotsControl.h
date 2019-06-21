#pragma once

#include <rqt_gui_cpp/plugin.h>

#include <robot_control/Robot.h>

#include <essentials/Identifier.h>
#include <SystemConfig.h>
#include <process_manager/RobotExecutableRegistry.h>
#include <engine/containers/AlicaEngineInfo.h>

#include <ui_RobotsControl.h>
#include <QDialog>
#include <QWidget>
#include <QtGui>

#include <chrono>
#include <mutex>
#include <queue>
#include <utility>

namespace robot_control
{
class Communication;

class RobotsControl : public rqt_gui_cpp::Plugin
{
    Q_OBJECT

  public:
    RobotsControl();
    virtual void initPlugin(qt_gui_cpp::PluginContext& context);
    virtual void shutdownPlugin();
    virtual void saveSettings(qt_gui_cpp::Settings& plugin_settings, qt_gui_cpp::Settings& instance_settings) const;
    virtual void restoreSettings(
            const qt_gui_cpp::Settings& plugin_settings, const qt_gui_cpp::Settings& instance_settings);

    void addRobot();
    void removeRobot();

    static std::chrono::duration<double> msgTimeOut;

    Ui::RobotControlWidget robotControlWidget_;
    QWidget* widget_;

    std::map<std::string, std::vector<std::pair<int, int>>> bundlesMap;
    essentials::RobotExecutableRegistry* pmRegistry;

  private:
    Communication* communication;
    essentials::SystemConfig* sc;

    std::map<const essentials::Identifier*, Robot*, essentials::IdentifierComparator> controlledRobotsMap;

    std::queue<std::pair<std::chrono::system_clock::time_point, process_manager::ProcessStats>>
            processStatMsgQueue;
    std::mutex processStatsMsgQueueMutex;
    std::queue<std::pair<std::chrono::system_clock::time_point, alica::AlicaEngineInfo>> alicaInfoMsgQueue;
    std::mutex alicaInfoMsgQueueMutex;


    void processMessages();
    void checkAndInit(const essentials::Identifier* robotId);

    QTimer* guiUpdateTimer;

  public Q_SLOTS:
    void run();
    void updateGUI();
    void showContextMenu(const QPoint& pos);
};

} // namespace robot_control
