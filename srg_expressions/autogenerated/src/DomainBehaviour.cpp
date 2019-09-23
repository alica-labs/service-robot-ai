#include "DomainBehaviour.h"
/*PROTECTED REGION ID(domainBehaviourSrcHeaders) ENABLED START*/
#include <srg/Robot.h>
#include <srg/SRGWorldModel.h>
/*PROTECTED REGION END*/

namespace alica {
DomainBehaviour::DomainBehaviour(std::string name) : BasicBehaviour(name) {
  /*PROTECTED REGION ID(domainBehaviourConstructor) ENABLED START*/
  this->robot = srg::Robot::getInstance();
  this->wm = srg::SRGWorldModel::getInstance();
  /*PROTECTED REGION END*/
}

DomainBehaviour::~DomainBehaviour() {
  /*PROTECTED REGION ID(domainBehaviourDestructor) ENABLED START*/
  /*PROTECTED REGION END*/
}

/*PROTECTED REGION ID(domainBehaviourMethods) ENABLED START*/
/*PROTECTED REGION END*/
} /* namespace alica */
