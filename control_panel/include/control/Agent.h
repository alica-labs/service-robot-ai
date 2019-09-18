#pragma once

#include <process_manager/containers/ProcessStats.h>
#include <essentials/IdentifierConstPtr.h>

#include <QGroupBox>
#include <QLayout>

#include<chrono>

namespace Ui
{
    class Agent;
}

namespace control {
    class Process;
    class ControlPanel;
    class ExecutableRegistry;
    class Agent : public QObject
    {
    Q_OBJECT

    public:
        Agent(essentials::IdentifierConstPtr id, ControlPanel* controlPanel);
        Agent(essentials::IdentifierConstPtr id, std::string agentName, ControlPanel* controlPanel);
        virtual ~Agent();



        void update(std::pair<std::chrono::system_clock::time_point, process_manager::ProcessStats> timePstsPair);
        void addExec(QWidget* exec);

        essentials::IdentifierConstPtr getAgentID();
        std::string getName();

        // GUI Methods and Members
        void updateGUI(std::chrono::system_clock::time_point now);
        void toggle();
        bool isShown();
        void show();
        void hide();

    public Q_SLOTS:
        void handleAgentCommandBtnClicked(bool checked);

    private:
        void updateGUI();

        bool shown;
        ControlPanel* controlPanel;
        std::chrono::duration<double> msgTimeOut;
        essentials::IdentifierConstPtr hostID;
        essentials::IdentifierConstPtr agentID;
        std::string agentName;
        QGroupBox* agentGroupBox;
        Ui::Agent* uiAgent;
        std::chrono::system_clock::time_point timeLastMsgReceived;
        std::map<int, Process*> processes;
    };
}

