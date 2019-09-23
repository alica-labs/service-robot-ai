#include "BehaviourCreator.h"
#include "Behaviours/Close.h"
#include "Behaviours/Move.h"
#include "Behaviours/Open.h"
#include "Behaviours/Pickup.h"
#include "Behaviours/Putdown.h"
#include "Behaviours/Spawn.h"
#include "Behaviours/Stop.h"
#include "Behaviours/Test.h"
#include "Behaviours/TestCodeGen.h"
#include "engine/BasicBehaviour.h"

namespace alica {

BehaviourCreator::BehaviourCreator() {}

BehaviourCreator::~BehaviourCreator() {}

std::shared_ptr<BasicBehaviour> BehaviourCreator::createBehaviour(long behaviourId)
{
    switch (behaviourId) {
    case 1555602210283:
        return std::make_shared<Stop>();
        break;
    case 1558533534825:
        return std::make_shared<Spawn>();
        break;
    case 1559635479527:
        return std::make_shared<Test>();
        break;
    case 1568801673599:
        return std::make_shared<Pickup>();
        break;
    case 1568801687587:
        return std::make_shared<Putdown>();
        break;
    case 1568801700485:
        return std::make_shared<Open>();
        break;
    case 1568801712239:
        return std::make_shared<Close>();
        break;
    case 1568825137528:
        return std::make_shared<Move>();
        break;
    default:
        std::cerr << "BehaviourCreator: Unknown behaviour requested: " << behaviourId << std::endl;
        throw new std::exception();
        break;
    }
}
} // namespace alica
