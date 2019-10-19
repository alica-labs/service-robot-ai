#include "Behaviours/Close.h"
#include <memory>

/*PROTECTED REGION ID(inccpp1568801712239) ENABLED START*/
#include <srg/Robot.h>
#include <srg/SRGWorldModel.h>
/*PROTECTED REGION END*/

namespace alica
{
/*PROTECTED REGION ID(staticVars1568801712239) ENABLED START*/
// initialise static variables here
/*PROTECTED REGION END*/

Close::Close()
        : DomainBehaviour("Close")
{
    /*PROTECTED REGION ID(con1568801712239) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
Close::~Close()
{
    /*PROTECTED REGION ID(dcon1568801712239) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
void Close::run(void* msg)
{
    /*PROTECTED REGION ID(run1568801712239) ENABLED START*/
    // Add additional options here
    std::cout << "Close called";
    this->robot->manipulate(this->wm->getOwnId(), srgsim::SimCommand::Action::CLOSE);
    /*PROTECTED REGION END*/
}
void Close::initialiseParameters()
{
    /*PROTECTED REGION ID(initialiseParameters1568801712239) ENABLED START*/
    // Add additional options here

    /*PROTECTED REGION END*/
}
/*PROTECTED REGION ID(methods1568801712239) ENABLED START*/
// Add additional options here
/*PROTECTED REGION END*/

} /* namespace alica */
