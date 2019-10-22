#include "Behaviours/Manipulate.h"
#include <memory>

/*PROTECTED REGION ID(inccpp1571687572903) ENABLED START*/
#include <srg/Robot.h>
#include <srg/SRGWorldModel.h>
#include <srg/dialogue/CommandHandler.h>
#include <srg/robot/Movement.h>
/*PROTECTED REGION END*/

namespace alica
{
/*PROTECTED REGION ID(staticVars1571687572903) ENABLED START*/
// initialise static variables here
/*PROTECTED REGION END*/

Manipulate::Manipulate()
        : DomainBehaviour("Manipulate")
{
    /*PROTECTED REGION ID(con1571687572903) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
Manipulate::~Manipulate()
{
    /*PROTECTED REGION ID(dcon1571687572903) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
void Manipulate::run(void* msg)
{
    /*PROTECTED REGION ID(run1571687572903) ENABLED START*/
    if (this->isSuccess()) {
        return;
    }

    auto currentCoordinate = this->wm->sRGSimData.getOwnPositionBuffer().getLastValidContent();

    if (!activeCommand.has_value()) {
        std::cout << "Manipulate::run(): Current command has no valid value!" << std::endl;
        return;
    }

    this->robot->manipulate(activeCommand->text);

    // TODO: successful execution of action should depend on perceptions...
    std::cout << "Manipulate::run(): Command successful: '" << activeCommand->text << std::endl;
    this->wm->dialogueManager.commandHandler->commandSuccessful(activeCommand->actID);
    this->setSuccess();
    /*PROTECTED REGION END*/
}
void Manipulate::initialiseParameters()
{
    /*PROTECTED REGION ID(initialiseParameters1571687572903) ENABLED START*/
    activeCommand = this->wm->dialogueManager.commandHandler->getActiveCommand();
    /*PROTECTED REGION END*/
}
/*PROTECTED REGION ID(methods1571687572903) ENABLED START*/
// Add additional options here
/*PROTECTED REGION END*/

} /* namespace alica */
