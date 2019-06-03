#include "DomainBehaviour.h"
/*PROTECTED REGION ID(domainBehaviourSrcHeaders) ENABLED START*/
#include <srg/SRGWorldModel.h>
#include <srg/Robot.h>
/*PROTECTED REGION END*/

namespace alica
{
DomainBehaviour::DomainBehaviour(std::string name)
        : BasicBehaviour(name)
{
    /*PROTECTED REGION ID(domainBehaviourConstructor) ENABLED START*/
    this->robot = new srg::Robot(srg::SRGWorldModel::getInstance());
    /*PROTECTED REGION END*/
}

DomainBehaviour::~DomainBehaviour()
{
    /*PROTECTED REGION ID(domainBehaviourDestructor) ENABLED START*/
    delete this->robot;
    /*PROTECTED REGION END*/
}

/*PROTECTED REGION ID(domainBehaviourMethods) ENABLED START*/
/*PROTECTED REGION END*/
} /* namespace alica */
