#include "ConditionCreator.h"
#include "Behaviours/GenerateTasks1575291385685.h"
#include "Behaviours/Manipulate1571687572903.h"
#include "Behaviours/Move1568825137528.h"
#include "Behaviours/Search1573419059418.h"
#include "Behaviours/Spawn1558533534825.h"
#include "Behaviours/Stop1555602210283.h"
#include "Behaviours/Test1559635479527.h"
#include "Human1575294066871.h"
#include "Operate1575655614484.h"
#include "Serve1568825275605.h"
#include "ServeMaster1555601736192.h"
#include "Transport1573418869596.h"

using namespace alicaAutogenerated;
namespace alica
{

ConditionCreator::ConditionCreator() {}
ConditionCreator::~ConditionCreator() {}

std::shared_ptr<BasicCondition> ConditionCreator::createConditions(long conditionConfId)
{
    switch (conditionConfId) {
    case 1558533620473:
        return std::make_shared<PreCondition1558533620473>();
        break;
    case 1558533654035:
        return std::make_shared<PreCondition1558533654035>();
        break;
    case 1558533667052:
        return std::make_shared<PreCondition1558533667052>();
        break;
    case 1558533725689:
        return std::make_shared<PreCondition1558533725689>();
        break;
    case 1560934538125:
        return std::make_shared<PreCondition1560934538125>();
        break;
    case 1568825457853:
        return std::make_shared<PreCondition1568825457853>();
        break;
    case 1568825476581:
        return std::make_shared<PreCondition1568825476581>();
        break;
    case 1571661864299:
        return std::make_shared<PreCondition1571661864299>();
        break;
    case 1571661980674:
        return std::make_shared<PreCondition1571661980674>();
        break;
    case 1573418732991:
        return std::make_shared<PreCondition1573418732991>();
        break;
    case 1573418838905:
        return std::make_shared<PreCondition1573418838905>();
        break;
    case 1573419765536:
        return std::make_shared<PreCondition1573419765536>();
        break;
    case 1573419800282:
        return std::make_shared<PreCondition1573419800282>();
        break;
    case 1573419810456:
        return std::make_shared<PreCondition1573419810456>();
        break;
    case 1573419828606:
        return std::make_shared<PreCondition1573419828606>();
        break;
    case 1573419841944:
        return std::make_shared<PreCondition1573419841944>();
        break;
    default:
        std::cerr << "ConditionCreator: Unknown condition id requested: " << conditionConfId << std::endl;
        throw new std::exception();
        break;
    }
}
} // namespace alica
