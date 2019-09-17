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
        virtual ~Agent();


        essentials::IdentifierConstPtr getAgentID();
        void update(std::chrono::system_clock::time_point now);
        void update(std::pair<std::chrono::system_clock::time_point, process_manager::ProcessStats> timePstsPair);
        void addExec(QWidget* exec);

    public Q_SLOTS:
        void handleAgentCommandBtnClicked(bool checked);

    private:
        void updateName();


        ControlPanel* controlPanel;
        std::chrono::duration<double> msgTimeOut;
        essentials::IdentifierConstPtr hostID;
        essentials::IdentifierConstPtr agentID;
        QGroupBox* agentGroupBox;
        Ui::Agent* uiAgent;
        std::chrono::system_clock::time_point timeLastMsgReceived;
        std::map<int, Process*> processes;
    };
}

