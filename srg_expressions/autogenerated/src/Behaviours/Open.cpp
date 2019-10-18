#include "Behaviours/Open.h"
#include <memory>

/*PROTECTED REGION ID(inccpp1568801700485) ENABLED START*/
// Add additional includes here
/*PROTECTED REGION END*/

namespace alica
{
/*PROTECTED REGION ID(staticVars1568801700485) ENABLED START*/
// initialise static variables here
/*PROTECTED REGION END*/

Open::Open()
        : DomainBehaviour("Open")
{
    /*PROTECTED REGION ID(con1568801700485) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
Open::~Open()
{
    /*PROTECTED REGION ID(dcon1568801700485) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
void Open::run(void* msg)
{
    /*PROTECTED REGION ID(run1568801700485) ENABLED START*/
    // Add additional options here
    std::cout << "Open called.\n";
    this->robot->open();
    /*PROTECTED REGION END*/
}
void Open::initialiseParameters()
{
    /*PROTECTED REGION ID(initialiseParameters1568801700485) ENABLED START*/
    // Add additional options here

    /*PROTECTED REGION END*/
}
/*PROTECTED REGION ID(methods1568801700485) ENABLED START*/
// Add additional options here
/*PROTECTED REGION END*/

} /* namespace alica */
