#include "BehaviourCreator.h"
#include "Behaviours/Manipulate.h"
#include "Behaviours/Move.h"
#include "Behaviours/Search.h"
#include "Behaviours/Spawn.h"
#include "Behaviours/Stop.h"
#include "Behaviours/Test.h"
#include "engine/BasicBehaviour.h"

namespace alica
{

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
    case 1568825137528:
        return std::make_shared<Move>();
        break;
    case 1571687572903:
        return std::make_shared<Manipulate>();
        break;
    case 1573419059418:
        return std::make_shared<Search>();
        break;
    default:
        std::cerr << "BehaviourCreator: Unknown behaviour requested: " << behaviourId << std::endl;
        throw new std::exception();
        break;
    }
}
} // namespace alica
