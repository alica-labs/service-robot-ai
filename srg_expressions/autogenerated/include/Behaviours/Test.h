#pragma once

#include "DomainBehaviour.h"
/*PROTECTED REGION ID(inc1559635479527) ENABLED START*/
// Add additional includes here
/*PROTECTED REGION END*/

namespace alica
{
class Test : public DomainBehaviour
{
public:
    Test();
    virtual ~Test();
    virtual void run(void* msg);
    /*PROTECTED REGION ID(pub1559635479527) ENABLED START*/
    // Add additional protected methods here
    /*PROTECTED REGION END*/
protected:
    virtual void initialiseParameters();
    /*PROTECTED REGION ID(pro1559635479527) ENABLED START*/
    // Add additional protected methods here
    /*PROTECTED REGION END*/
private:
    /*PROTECTED REGION ID(prv1559635479527) ENABLED START*/
    // Add additional private methods here
    /*PROTECTED REGION END*/
};
} /* namespace alica */
