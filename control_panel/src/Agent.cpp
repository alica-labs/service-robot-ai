#include "control/Agent.h"

namespace control
{
Agent::Agent(essentials::IdentifierConstPtr id, QLayout* parent)
        : id(id)
{
    this->uiBox = new QGroupBox();
    this->uiBox->setTitle(QString("Name of Agent!"));
    parent->addWidget(this->uiBox);
}

Agent::~Agent() {}

essentials::IdentifierConstPtr Agent::getID()
{
    return this->id;
}

void Agent::addToUI(QLayout& layout)
{
    layout.addWidget(this->uiBox);
}

void Agent::update(std::pair<std::chrono::system_clock::time_point, process_manager::ProcessStats> timePstsPair) {
//    if (timePstsPair.first)
    ((QLayout*)this->uiBox->parent())->removeWidget(this->uiBox);
}
} // namespace control