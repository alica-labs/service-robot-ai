#pragma once

#include <process_manager/containers/ProcessStats.h>
#include <essentials/IdentifierConstPtr.h>

#include <QGroupBox>
#include <QLayout>

#include<chrono>

namespace control {
    class Agent {
    public:
        Agent(essentials::IdentifierConstPtr id, QLayout* parent);
        virtual ~Agent();

        essentials::IdentifierConstPtr getID();
        void addToUI(QLayout& layout);

        void update(std::pair<std::chrono::system_clock::time_point, process_manager::ProcessStats> timePstsPair);
    private:
        essentials::IdentifierConstPtr id;
        QGroupBox* uiBox;
    };
}

