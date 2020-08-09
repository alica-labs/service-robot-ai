#include "Human1575294066871.h"
/*PROTECTED REGION ID(eph1575294066871) ENABLED START*/
// Add additional options here
/*PROTECTED REGION END*/

namespace alica
{
// Plan:Human
/**
 * Task: GenerateTasks  -> EntryPoint-ID: 1575294081333
 */
std::shared_ptr<UtilityFunction> UtilityFunction1575294066871::getUtilityFunction(Plan* plan)
{
    /*PROTECTED REGION ID(1575294066871) ENABLED START*/
    std::shared_ptr<UtilityFunction> defaultFunction = std::make_shared<DefaultUtilityFunction>(plan);
    return defaultFunction;
    /*PROTECTED REGION END*/
}
} // namespace alica
