#pragma once

#include <engine/BasicConstraint.h>
#include <iostream>
#include <memory>

using namespace alica;

namespace alica {
class ProblemDescriptor;
class RunningPlan;
} // namespace alica

namespace alicaAutogenerated {

class Constraint1558533725689 : public BasicConstraint {
  void getConstraint(std::shared_ptr<ProblemDescriptor> c,
                     std::shared_ptr<RunningPlan> rp);
};
} // namespace alicaAutogenerated
