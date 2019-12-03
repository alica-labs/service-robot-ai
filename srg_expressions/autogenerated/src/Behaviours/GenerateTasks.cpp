#include "Behaviours/GenerateTasks.h"
#include <memory>

/*PROTECTED REGION ID(inccpp1575291385685) ENABLED START*/
#include <srg/Agent.h>
/*PROTECTED REGION END*/

namespace alica
{
/*PROTECTED REGION ID(staticVars1575291385685) ENABLED START*/
// initialise static variables here
/*PROTECTED REGION END*/

GenerateTasks::GenerateTasks()
        : DomainBehaviour("GenerateTasks")
{
    /*PROTECTED REGION ID(con1575291385685) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
GenerateTasks::~GenerateTasks()
{
    /*PROTECTED REGION ID(dcon1575291385685) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
void GenerateTasks::run(void* msg)
{
    /*PROTECTED REGION ID(run1575291385685) ENABLED START*/
    /*PROTECTED REGION END*/
}
void GenerateTasks::initialiseParameters()
{
    /*PROTECTED REGION ID(initialiseParameters1575291385685) ENABLED START*/
    sa.senderID = this->wm->getOwnId();
    for (essentials::IdentifierConstPtr agentId : this->wm->getEngine()->getTeamManager()->getActiveAgentIds()) {
        if (agentId != this->wm->getOwnId()) {
            sa.receiverID = agentId;
            break;
        }
    }
    sa.actID = this->wm->getEngine()->getIdManager()->generateID();
    sa.previousActID = this->wm->getEngine()->getIdManager()->getWildcardID();
    sa.type = srg::agent::SpeechType::command;
    sa.text = "transport CupBlue 5,5";
    this->agent->speak(sa);
    /*PROTECTED REGION END*/
}
/*PROTECTED REGION ID(methods1575291385685) ENABLED START*/
// Add additional options here
/*PROTECTED REGION END*/

} /* namespace alica */
