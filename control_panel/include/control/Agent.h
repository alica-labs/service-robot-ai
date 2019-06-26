#pragma once

#include <essentials/IdentifierConstPtr.h>

#include <QGroupBox>
#include <QLayout>

namespace control {
    class Agent {
    public:
        Agent(essentials::IdentifierConstPtr id, QLayout* parent);
        virtual ~Agent();

        essentials::IdentifierConstPtr getID();
        void addToUI(QLayout& layout);
    private:
        essentials::IdentifierConstPtr id;
        QGroupBox* uiBox;
    };
}

