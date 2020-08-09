#pragma once

#include <iostream>

namespace essentials {
    class IdentifierConstPtr;
}

namespace alica
{
class AlicaContext;
class BehaviourCreator;
class ConditionCreator;
class UtilityFunctionCreator;
class ConstraintCreator;
} // namespace alica

namespace srg
{

class SRGWorldModel;

class Base
{
public:
    Base(std::string roleSetName, std::string masterPlanName, std::string roleSetDir, essentials::IdentifierConstPtr identifier);
    virtual ~Base();

    void start();

    static void simSigintHandler(int sig);
    static bool isRunning();
    static bool running;

    alica::AlicaContext* ac;
    SRGWorldModel* wm;
};

} // namespace srg
