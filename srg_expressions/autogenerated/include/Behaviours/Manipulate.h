#pragma once

#include "DomainBehaviour.h"
/*PROTECTED REGION ID(inc1571687572903) ENABLED START*/
#include <control/containers/SpeechAct.h>
#include <nonstd/optional.hpp>
#include <srg/tasks/Task.h>
#include <srg/world/Coordinate.h>
#include <supplementary/InformationElement.h>
/*PROTECTED REGION END*/

namespace alica
{
class Manipulate : public DomainBehaviour
{
public:
    Manipulate();
    virtual ~Manipulate();
    virtual void run(void* msg);
    /*PROTECTED REGION ID(pub1571687572903) ENABLED START*/
    // Add additional protected methods here
    /*PROTECTED REGION END*/
protected:
    virtual void initialiseParameters();
    /*PROTECTED REGION ID(pro1571687572903) ENABLED START*/
    // Add additional protected methods here
    /*PROTECTED REGION END*/
private:
    /*PROTECTED REGION ID(prv1571687572903) ENABLED START*/
    std::shared_ptr<const supplementary::InformationElement<srg::tasks::Task*>> task;
    srg::tasks::Task* activeTask;
    /*PROTECTED REGION END*/
};
} /* namespace alica */
