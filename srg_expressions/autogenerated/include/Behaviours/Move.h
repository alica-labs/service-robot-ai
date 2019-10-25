#pragma once

#include "DomainBehaviour.h"
/*PROTECTED REGION ID(inc1568825137528) ENABLED START*/
#include <nonstd/optional.hpp>
#include <srgsim/containers/Coordinate.h>
#include <srg/dialogue/Task.h>
/*PROTECTED REGION END*/

namespace alica
{
class Move : public DomainBehaviour
{
public:
    Move();
    virtual ~Move();
    virtual void run(void* msg);
    /*PROTECTED REGION ID(pub1568825137528) ENABLED START*/
    int counter;
    /*PROTECTED REGION END*/
protected:
    virtual void initialiseParameters();
    /*PROTECTED REGION ID(pro1568825137528) ENABLED START*/
    // Add additional protected methods here
    /*PROTECTED REGION END*/
private:
    /*PROTECTED REGION ID(prv1568825137528) ENABLED START*/
    nonstd::optional<srg::dialogue::Task> activeTask;
    /*PROTECTED REGION END*/
};
} /* namespace alica */
