#include "Operate1575655614484.h"
/*PROTECTED REGION ID(eph1575655614484) ENABLED START*/
// Add additional options here
/*PROTECTED REGION END*/

namespace alica
{
// Plan:Operate
/**
 * Task: Serve  -> EntryPoint-ID: 1575655644510
 * Task: GenerateTasks  -> EntryPoint-ID: 1575655650043
 */
std::shared_ptr<UtilityFunction> UtilityFunction1575655614484::getUtilityFunction(Plan* plan)
{
    /*PROTECTED REGION ID(1575655614484) ENABLED START*/
    std::shared_ptr<UtilityFunction> defaultFunction = std::make_shared<DefaultUtilityFunction>(plan);
    return defaultFunction;
    /*PROTECTED REGION END*/
}
} // namespace alica
