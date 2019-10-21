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
    std::cout << "Manipulate::run() called!" << std::endl;
    auto currentCoordinate = this->wm->sRGSimData.getOwnPositionBuffer().getLastValidContent();

    if (!activeCommand.has_value()) {
        std::cout << "Manipulate::run(): Current command has no valid value!" << std::endl;
        return;
    }

    srgsim::SimCommand::Action action;
    size_t cmdIdx = activeCommand->text.find("open");
    if (cmdIdx != std::string::npos) {
        action = srgsim::SimCommand::Action::OPEN;
    } else {
        cmdIdx = activeCommand->text.find("close");
        if (cmdIdx != std::string::npos) {
            action = srgsim::SimCommand::Action::CLOSE;
        } else {
            cmdIdx = activeCommand->text.find("pick");
            if (cmdIdx != std::string::npos) {
                action = srgsim::SimCommand::Action::PICKUP;
            } else {
                cmdIdx = activeCommand->text.find("put");
                if (cmdIdx != std::string::npos) {
                    action = srgsim::SimCommand::Action::PUTDOWN;
                } else {
                    std::cout << "Manipulate::run(): Current command is unknown: " << activeCommand->text << std::endl;
                    return;
                }
            }
        }
    }

    size_t idIdx = activeCommand->text.find_last_of(" ");
    uint32_t idInt = std::stoi(activeCommand->text.substr(idIdx));
    essentials::IdentifierConstPtr id = this->wm->getEngine()->getId<uint32_t>(idInt);
    if (firstRun) {
        this->robot->manipulate(id, action);
    } else {
        std::cout << "Manipulate::run(): Command successful: " << activeCommand->text << std::endl;
        this->wm->dialogueManager.commandHandler->commandSuccessful(this->activeCommand->actID);
        this->setSuccess();
        return;
    }
    /*PROTECTED REGION END*/
}
void Manipulate::initialiseParameters()
{
    /*PROTECTED REGION ID(initialiseParameters1571687572903) ENABLED START*/
    this->activeCommand = this->wm->dialogueManager.commandHandler->getActiveCommand();
    firstRun = true;
    /*PROTECTED REGION END*/
}
/*PROTECTED REGION ID(methods1571687572903) ENABLED START*/
// Add additional options here
/*PROTECTED REGION END*/

} /* namespace alica */
