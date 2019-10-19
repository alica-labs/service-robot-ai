#include "Behaviours/Move.h"
#include <memory>

/*PROTECTED REGION ID(inccpp1568825137528) ENABLED START*/
#include <srg/Robot.h>
#include <srg/SRGWorldModel.h>
#include <srg/robot/Movement.h>
#include <srg/dialogue/CommandHandler.h>
/*PROTECTED REGION END*/

namespace alica
{
/*PROTECTED REGION ID(staticVars1568825137528) ENABLED START*/
// initialise static variables here
/*PROTECTED REGION END*/

Move::Move()
        : DomainBehaviour("Move")
{
    /*PROTECTED REGION ID(con1568825137528) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
Move::~Move()
{
    /*PROTECTED REGION ID(dcon1568825137528) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
void Move::run(void* msg)
{
    /*PROTECTED REGION ID(run1568825137528) ENABLED START*/
    std::cout << "Move::run() called!" << std::endl;
    auto currentCoordinate = this->wm->sRGSimData.getOwnPositionBuffer().getLastValidContent();

    if (!activeCommand.has_value()) {
        std::cout << "Move::run(): Current command has no valid value!" << std::endl;
        return;
    }

    size_t moveIdx = activeCommand->text.find("move");
    if (moveIdx == std::string::npos) {
        std::cout << "Move::run(): Current command is no move command!" << std::endl;
        return;
    }

    size_t commaIdx = activeCommand->text.find(",", moveIdx);
    if (commaIdx == std::string::npos) {
        std::cout << "Move::run(): Current move command has no comma!" << std::endl;
        return;
    }

    int xCoord = std::stoi(activeCommand->text.substr(moveIdx+4, commaIdx-(moveIdx+4)));
    int yCoord = std::stoi(activeCommand->text.substr(commaIdx+1));

    srgsim::Coordinate goalCoordinate = srgsim::Coordinate(xCoord, yCoord);
    if (currentCoordinate.has_value() && currentCoordinate.value() == goalCoordinate) {
        std::cout << "Move::run(): Reached goal: " << goalCoordinate << std::endl;
        this->wm->dialogueManager.commandHandler->commandSuccessful(this->activeCommand->actID);
        this->setSuccess();
        return;
    } else if (firstRun || this->startCoordinate != currentCoordinate) {
        startCoordinate = currentCoordinate;
        firstRun = false;
        this->robot->move(goalCoordinate);
    }
    /*PROTECTED REGION END*/
}
void Move::initialiseParameters()
{
    /*PROTECTED REGION ID(initialiseParameters1568825137528) ENABLED START*/
    this->startCoordinate = this->wm->sRGSimData.getOwnPositionBuffer().getLastValidContent();
    this->activeCommand = this->wm->dialogueManager.commandHandler->getActiveCommand();
    firstRun = true;
    /*PROTECTED REGION END*/
}
/*PROTECTED REGION ID(methods1568825137528) ENABLED START*/
// Add additional options here
/*PROTECTED REGION END*/

} /* namespace alica */
