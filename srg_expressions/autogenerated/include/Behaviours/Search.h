#pragma once

#include "DomainBehaviour.h"
/*PROTECTED REGION ID(inc1573419059418) ENABLED START*/
//Add additional includes here
/*PROTECTED REGION END*/

namespace alica
{
    class Search : public DomainBehaviour
    {
        public:
            Search();
            virtual ~Search();
            virtual void run(void* msg);
            /*PROTECTED REGION ID(pub1573419059418) ENABLED START*/
            //Add additional protected methods here
            /*PROTECTED REGION END*/
        protected:
            virtual void initialiseParameters();
            /*PROTECTED REGION ID(pro1573419059418) ENABLED START*/
            //Add additional protected methods here
            /*PROTECTED REGION END*/
        private:
        /*PROTECTED REGION ID(prv1573419059418) ENABLED START*/
            //Add additional private methods here
        /*PROTECTED REGION END*/
    };
} /* namespace alica */
