#include "Behaviours/Spawn.h"
#include <memory>

/*PROTECTED REGION ID(inccpp1558533534825) ENABLED START*/
#include <srg/Agent.h>
/*PROTECTED REGION END*/

namespace alica
{
/*PROTECTED REGION ID(staticVars1558533534825) ENABLED START*/
// initialise static variables here
/*PROTECTED REGION END*/

Spawn::Spawn()
        : DomainBehaviour("Spawn")
{
    /*PROTECTED REGION ID(con1558533534825) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
Spawn::~Spawn()
{
    /*PROTECTED REGION ID(dcon1558533534825) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
void Spawn::run(void* msg)
{
    /*PROTECTED REGION ID(run1558533534825) ENABLED START*/
    std::cout << "Spawn is called!" << std::endl;
    this->agent->spawn();
    /*PROTECTED REGION END*/
}
void Spawn::initialiseParameters()
{
    /*PROTECTED REGION ID(initialiseParameters1558533534825) ENABLED START*/
    // Add additional options here

    /*PROTECTED REGION END*/
}
/*PROTECTED REGION ID(methods1558533534825) ENABLED START*/
// Add additional options here
/*PROTECTED REGION END*/

} /* namespace alica */
