#include "UtilityFunctionCreator.h"
#include "Human1575294066871.h"
#include "Operate1575655614484.h"
#include "Serve1568825275605.h"
#include "ServeMaster1555601736192.h"
#include "Transport1573418869596.h"
#include <iostream>

namespace alica
{

UtilityFunctionCreator::~UtilityFunctionCreator() {}

UtilityFunctionCreator::UtilityFunctionCreator() {}

std::shared_ptr<BasicUtilityFunction> UtilityFunctionCreator::createUtility(long utilityfunctionConfId)
{
    switch (utilityfunctionConfId) {
    case 1555601736192:
        return std::make_shared<UtilityFunction1555601736192>();
        break;
    case 1568825275605:
        return std::make_shared<UtilityFunction1568825275605>();
        break;
    case 1573418869596:
        return std::make_shared<UtilityFunction1573418869596>();
        break;
    case 1575294066871:
        return std::make_shared<UtilityFunction1575294066871>();
        break;
    case 1575655614484:
        return std::make_shared<UtilityFunction1575655614484>();
        break;
    default:
        std::cerr << "UtilityFunctionCreator: Unknown utility requested: " << utilityfunctionConfId << std::endl;
        throw new std::exception();
        break;
    }
}

} // namespace alica
