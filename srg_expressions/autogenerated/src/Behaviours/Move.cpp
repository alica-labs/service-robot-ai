#include "Behaviours/Move.h"
#include <memory>

/*PROTECTED REGION ID(inccpp1568825137528) ENABLED START*/
#include <srg/Robot.h>
#include <srg/SRGWorldModel.h>
#include <srg/robot/Movement.h>
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
    srgsim::Coordinate goalCoordinate = srgsim::Coordinate(1, 1);
    auto currentCoordinate = this->wm->sRGSimData.getOwnPositionBuffer().getLastValidContent();
    if (currentCoordinate.has_value() && currentCoordinate.value() == goalCoordinate) {
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
    firstRun = true;
    /*PROTECTED REGION END*/
}
/*PROTECTED REGION ID(methods1568825137528) ENABLED START*/
// Add additional options here
/*PROTECTED REGION END*/

} /* namespace alica */
