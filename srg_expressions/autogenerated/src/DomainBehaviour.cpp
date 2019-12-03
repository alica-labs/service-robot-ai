#include "DomainBehaviour.h"
/*PROTECTED REGION ID(domainBehaviourSrcHeaders) ENABLED START*/
#include <srg/Agent.h>
#include <srg/SRGWorldModel.h>
/*PROTECTED REGION END*/

namespace alica
{
DomainBehaviour::DomainBehaviour(std::string name)
        : BasicBehaviour(name)
{
    /*PROTECTED REGION ID(domainBehaviourConstructor) ENABLED START*/
    this->agent = srg::Agent::getInstance();
    this->wm = srg::SRGWorldModel::getInstance();
    /*PROTECTED REGION END*/
}

DomainBehaviour::~DomainBehaviour()
{
    /*PROTECTED REGION ID(domainBehaviourDestructor) ENABLED START*/
    /*PROTECTED REGION END*/
}

/*PROTECTED REGION ID(domainBehaviourMethods) ENABLED START*/
/*PROTECTED REGION END*/
} /* namespace alica */
