#include "Behaviours/Putdown.h"
#include <memory>

/*PROTECTED REGION ID(inccpp1568801687587) ENABLED START*/
#include <srg/Robot.h>
#include <srg/SRGWorldModel.h>
/*PROTECTED REGION END*/

namespace alica
{
/*PROTECTED REGION ID(staticVars1568801687587) ENABLED START*/
// initialise static variables here
/*PROTECTED REGION END*/

Putdown::Putdown()
        : DomainBehaviour("Putdown")
{
    /*PROTECTED REGION ID(con1568801687587) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
Putdown::~Putdown()
{
    /*PROTECTED REGION ID(dcon1568801687587) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
void Putdown::run(void* msg)
{
    /*PROTECTED REGION ID(run1568801687587) ENABLED START*/
    // Add additional options here
    std::cout << "Putdown called.\n";
    this->robot->manipulate(this->wm->getOwnId(), srgsim::SimCommand::Action::PUTDOWN);
    /*PROTECTED REGION END*/
}
void Putdown::initialiseParameters()
{
    /*PROTECTED REGION ID(initialiseParameters1568801687587) ENABLED START*/
    // Add additional options here

    /*PROTECTED REGION END*/
}
/*PROTECTED REGION ID(methods1568801687587) ENABLED START*/
// Add additional options here
/*PROTECTED REGION END*/

} /* namespace alica */
