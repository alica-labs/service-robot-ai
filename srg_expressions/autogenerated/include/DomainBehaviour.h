#pragma once

#include <engine/BasicBehaviour.h>
#include <string>
/*PROTECTED REGION ID(domainBehaviourHeaderHead) ENABLED START*/
namespace srg
{
class Robot;
class SRGWorldModel;
} // namespace srg
/*PROTECTED REGION END*/

namespace alica
{
    class DomainBehaviour : public BasicBehaviour
    {
        public:
        DomainBehaviour(std::string name);
        virtual ~DomainBehaviour();

        /*PROTECTED REGION ID(domainBehaviourClassDecl) ENABLED START*/
    srg::Robot* robot;
    srg::SRGWorldModel* wm;
        /*PROTECTED REGION END*/
    };
} /* namespace alica */
