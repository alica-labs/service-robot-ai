#pragma once

#include "DomainBehaviour.h"
/*PROTECTED REGION ID(inc1568801712239) ENABLED START*/
// Add additional includes here
/*PROTECTED REGION END*/

namespace alica
{
class Close : public DomainBehaviour
{
public:
    Close();
    virtual ~Close();
    virtual void run(void* msg);
    /*PROTECTED REGION ID(pub1568801712239) ENABLED START*/
    // Add additional protected methods here
    /*PROTECTED REGION END*/
protected:
    virtual void initialiseParameters();
    /*PROTECTED REGION ID(pro1568801712239) ENABLED START*/
    // Add additional protected methods here
    /*PROTECTED REGION END*/
private:
    /*PROTECTED REGION ID(prv1568801712239) ENABLED START*/
    // Add additional private methods here
    /*PROTECTED REGION END*/
};
} /* namespace alica */
