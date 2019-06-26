#include "control/Agent.h"

namespace control
{
Agent::Agent(essentials::IdentifierConstPtr id, QLayout* parent)
        : id(id)
{
    this->uiBox->setTitle(QString("Name of Agent!"));
    this->uiBox = new QGroupBox();
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
} // namespace control