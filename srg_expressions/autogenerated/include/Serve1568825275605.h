#pragma once

#include "DomainCondition.h"
#include <engine/BasicUtilityFunction.h>
#include <engine/DefaultUtilityFunction.h>
#include <engine/UtilityFunction.h>
/*PROTECTED REGION ID(incl1568825275605) ENABLED START*/
// Add additional includes here
/*PROTECTED REGION END*/
using namespace alica;

namespace alicaAutogenerated
{
/*PROTECTED REGION ID(meth1568825275605) ENABLED START*/
// Add additional options here
/*PROTECTED REGION END*/
class UtilityFunction1568825275605 : public BasicUtilityFunction
{
    shared_ptr<UtilityFunction> getUtilityFunction(Plan* plan);
};
class PreCondition1568825457853 : public DomainCondition
{
    bool evaluate(std::shared_ptr<RunningPlan> rp);
};
class PreCondition1568825476581 : public DomainCondition
{
    bool evaluate(std::shared_ptr<RunningPlan> rp);
};
} // namespace alicaAutogenerated
