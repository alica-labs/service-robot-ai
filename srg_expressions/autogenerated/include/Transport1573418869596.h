#pragma once

#include "DomainCondition.h"
#include <engine/BasicUtilityFunction.h>
#include <engine/UtilityFunction.h>
#include <engine/DefaultUtilityFunction.h>
/*PROTECTED REGION ID(incl1573418869596) ENABLED START*/
//Add additional includes here
/*PROTECTED REGION END*/
using namespace alica;

namespace alicaAutogenerated
{
    /*PROTECTED REGION ID(meth1573418869596) ENABLED START*/
    //Add additional options here
    /*PROTECTED REGION END*/
    class UtilityFunction1573418869596 : public BasicUtilityFunction
    {
        shared_ptr<UtilityFunction> getUtilityFunction(Plan* plan);
    };
    class PreCondition1573419746851 : public DomainCondition
    {
        bool evaluate(std::shared_ptr<RunningPlan> rp);
    };
    class PreCondition1573419765536 : public DomainCondition
    {
        bool evaluate(std::shared_ptr<RunningPlan> rp);
    };
    class PreCondition1573419810456 : public DomainCondition
    {
        bool evaluate(std::shared_ptr<RunningPlan> rp);
    };
    class PreCondition1573419828606 : public DomainCondition
    {
        bool evaluate(std::shared_ptr<RunningPlan> rp);
    };
    class PreCondition1573419841944 : public DomainCondition
    {
        bool evaluate(std::shared_ptr<RunningPlan> rp);
    };
    class PreCondition1573419800282 : public DomainCondition
    {
        bool evaluate(std::shared_ptr<RunningPlan> rp);
    };
} /* namespace alica */