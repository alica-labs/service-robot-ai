#pragma once

#include "DomainBehaviour.h"
/*PROTECTED REGION ID(inc1568801700485) ENABLED START*/
// Add additional includes here
/*PROTECTED REGION END*/

namespace alica {
class Open : public DomainBehaviour {
public:
  Open();
  virtual ~Open();
  virtual void run(void *msg);
  /*PROTECTED REGION ID(pub1568801700485) ENABLED START*/
  // Add additional protected methods here
  /*PROTECTED REGION END*/
protected:
  virtual void initialiseParameters();
  /*PROTECTED REGION ID(pro1568801700485) ENABLED START*/
  // Add additional protected methods here
  /*PROTECTED REGION END*/
private:
  /*PROTECTED REGION ID(prv1568801700485) ENABLED START*/
  // Add additional private methods here
  /*PROTECTED REGION END*/
};
} /* namespace alica */
