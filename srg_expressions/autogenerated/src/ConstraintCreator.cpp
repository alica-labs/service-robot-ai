#include "ConstraintCreator.h"

#include "Behaviours/constraints/GenerateTasks1575291385685Constraints.h"
#include "Behaviours/constraints/Manipulate1571687572903Constraints.h"
#include "Behaviours/constraints/Move1568825137528Constraints.h"
#include "Behaviours/constraints/Search1573419059418Constraints.h"
#include "Behaviours/constraints/Spawn1558533534825Constraints.h"
#include "Behaviours/constraints/Stop1555602210283Constraints.h"
#include "Behaviours/constraints/Test1559635479527Constraints.h"
#include "constraints/Human1575294066871Constraints.h"
#include "constraints/Operate1575655614484Constraints.h"
#include "constraints/Serve1568825275605Constraints.h"
#include "constraints/ServeMaster1555601736192Constraints.h"
#include "constraints/Transport1573418869596Constraints.h"

#include <iostream>

namespace alica
{

ConstraintCreator::ConstraintCreator() {}

ConstraintCreator::~ConstraintCreator() {}

std::shared_ptr<BasicConstraint> ConstraintCreator::createConstraint(long constraintConfId)
{
    switch (constraintConfId) {
    default:
        std::cerr << "ConstraintCreator: Unknown constraint requested: " << constraintConfId << std::endl;
        throw new std::exception();
        break;
    }
}

} // namespace alica
