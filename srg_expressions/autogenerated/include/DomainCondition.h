#pragma once

#include <engine/BasicCondition.h>
/*PROTECTED REGION ID(domainHeaderAdditional) ENABLED START*/
#include <srg/SRGWorldModel.h>
/*PROTECTED REGION END*/

namespace alica
{
    class DomainCondition : public BasicCondition
    {
        public:
        DomainCondition();
        virtual ~DomainCondition();

        /*PROTECTED REGION ID(domainHeader) ENABLED START*/
    // Add additional options here
    srg::SRGWorldModel* wm = srg::SRGWorldModel::getInstance();








        /*PROTECTED REGION END*/

protected:
                /*PROTECTED REGION ID(protectedDomainHeader) ENABLED START*/
    // Add additional options here








        /*PROTECTED REGION END*/

private:
                /*PROTECTED REGION ID(privateDomainHeader) ENABLED START*/
    // Add additional options here
        /*PROTECTED REGION END*/
    };
} /* namespace alica */
