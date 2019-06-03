#pragma once

#include <iostream>

namespace alica
{
class AlicaEngine;
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
    Base(std::string roleSetName, std::string masterPlanName, std::string roleSetDir);
    virtual ~Base();

    void start();

    static void simSigintHandler(int sig);
    static bool isRunning();
    static bool running;

    alica::AlicaEngine* ae;
    alica::BehaviourCreator* bc;
    alica::ConditionCreator* cc;
    alica::UtilityFunctionCreator* uc;
    alica::ConstraintCreator* crc;
    SRGWorldModel* wm;
};

} // namespace srg
