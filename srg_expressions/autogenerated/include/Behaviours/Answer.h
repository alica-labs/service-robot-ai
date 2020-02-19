#pragma once

#include "DomainBehaviour.h"
/*PROTECTED REGION ID(inc1581966891046) ENABLED START*/
// Add additional includes here
/*PROTECTED REGION END*/

namespace alica
{
class Answer : public DomainBehaviour
{
public:
    Answer();
    virtual ~Answer();
    virtual void run(void* msg);
    /*PROTECTED REGION ID(pub1581966891046) ENABLED START*/
    // Add additional protected methods here
    /*PROTECTED REGION END*/
protected:
    virtual void initialiseParameters();
    /*PROTECTED REGION ID(pro1581966891046) ENABLED START*/
    // Add additional protected methods here
    /*PROTECTED REGION END*/
private:
    /*PROTECTED REGION ID(prv1581966891046) ENABLED START*/
    // Add additional private methods here
    /*PROTECTED REGION END*/
};
} /* namespace alica */
