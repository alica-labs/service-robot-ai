#include "Behaviours/Stop.h"
#include <memory>

/*PROTECTED REGION ID(inccpp1555602210283) ENABLED START*/
#include <capnp/message.h>
/*PROTECTED REGION END*/

namespace alica
{
/*PROTECTED REGION ID(staticVars1555602210283) ENABLED START*/
// initialise static variables here
/*PROTECTED REGION END*/

Stop::Stop()
        : DomainBehaviour("Stop")
{
    /*PROTECTED REGION ID(con1555602210283) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
Stop::~Stop()
{
    /*PROTECTED REGION ID(dcon1555602210283) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
void Stop::run(void* msg)
{
    /*PROTECTED REGION ID(run1555602210283) ENABLED START*/
    std::cout << "Stop is called!" << std::endl;

    // init builder
    ::capnp::MallocMessageBuilder msgBuilder;
//    capnzero::String::Builder beaconMsgBuilder = msgBuilder.initRoot<srgsim::Command>();

    /*PROTECTED REGION END*/
}
void Stop::initialiseParameters()
{
    /*PROTECTED REGION ID(initialiseParameters1555602210283) ENABLED START*/
    // Add additional options here

    /*PROTECTED REGION END*/
}
/*PROTECTED REGION ID(methods1555602210283) ENABLED START*/
// Add additional options here
/*PROTECTED REGION END*/

} /* namespace alica */
