#pragma once

#include "DomainBehaviour.h"
/*PROTECTED REGION ID(inc1568801687587) ENABLED START*/
// Add additional includes here
/*PROTECTED REGION END*/

namespace alica {
class Putdown : public DomainBehaviour {
public:
  Putdown();
  virtual ~Putdown();
  virtual void run(void *msg);
  /*PROTECTED REGION ID(pub1568801687587) ENABLED START*/
  // Add additional protected methods here
  /*PROTECTED REGION END*/
protected:
  virtual void initialiseParameters();
  /*PROTECTED REGION ID(pro1568801687587) ENABLED START*/
  // Add additional protected methods here
  /*PROTECTED REGION END*/
private:
  /*PROTECTED REGION ID(prv1568801687587) ENABLED START*/
  // Add additional private methods here
  /*PROTECTED REGION END*/
};
} /* namespace alica */
