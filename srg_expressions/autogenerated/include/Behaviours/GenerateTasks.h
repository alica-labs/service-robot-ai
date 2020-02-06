#pragma once

#include "DomainBehaviour.h"
/*PROTECTED REGION ID(inc1575291385685) ENABLED START*/
// Add additional includes here
/*PROTECTED REGION END*/

namespace alica
{
class GenerateTasks : public DomainBehaviour
{
public:
    GenerateTasks();
    virtual ~GenerateTasks();
    virtual void run(void* msg);
    /*PROTECTED REGION ID(pub1575291385685) ENABLED START*/
    // Add additional protected methods here
    /*PROTECTED REGION END*/
protected:
    virtual void initialiseParameters();
    /*PROTECTED REGION ID(pro1575291385685) ENABLED START*/
    // Add additional protected methods here
    /*PROTECTED REGION END*/
private:
    /*PROTECTED REGION ID(prv1575291385685) ENABLED START*/
    // Add additional private methods here
    /*PROTECTED REGION END*/
};
} /* namespace alica */
