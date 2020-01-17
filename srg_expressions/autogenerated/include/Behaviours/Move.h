#pragma once

#include "DomainBehaviour.h"
/*PROTECTED REGION ID(inc1568825137528) ENABLED START*/
#include <nonstd/optional.hpp>
#include <srg/tasks/TaskSequence.h>
#include <srg/world/Coordinate.h>
#include <supplementary/InformationElement.h>
/*PROTECTED REGION END*/

namespace alica {
class Move : public DomainBehaviour {
public:
  Move();
  virtual ~Move();
  virtual void run(void *msg);
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
    std::shared_ptr<srg::tasks::TaskSequence> taskSequence;
    srg::tasks::Task* activeTask;
    /*PROTECTED REGION END*/
};
} /* namespace alica */
