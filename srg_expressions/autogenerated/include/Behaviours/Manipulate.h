#pragma once

#include "DomainBehaviour.h"
/*PROTECTED REGION ID(inc1571687572903) ENABLED START*/
#include <control/containers/SpeechAct.h>
#include <nonstd/optional.hpp>
#include <srgsim/containers/Coordinate.h>
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
    nonstd::optional<control::SpeechAct> activeCommand;
    bool firstRun;
    /*PROTECTED REGION END*/
};
} /* namespace alica */
