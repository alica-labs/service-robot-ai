#pragma once

#include "DomainBehaviour.h"
#include <srg/agent/containers/SpeechAct.h>
#include <srg/world/Coordinate.h>
/*PROTECTED REGION ID(inc1575291385685) ENABLED START*/
namespace srg
{
namespace world
{
class Room;
}
} // namespace srg
/*PROTECTED REGION END*/

namespace alica
{
class GenerateTasks : public DomainBehaviour
{
public:
    GenerateTasks();
    virtual ~GenerateTasks();
    virtual void run(void* msg);
    /*PROTECTED REGION ID(pub1575291385685) ENABLED START*/
    // Add additional protected methods here
    /*PROTECTED REGION END*/
protected:
    virtual void initialiseParameters();
    /*PROTECTED REGION ID(pro1575291385685) ENABLED START*/
    // Add additional protected methods here
    /*PROTECTED REGION END*/
private:
    /*PROTECTED REGION ID(prv1575291385685) ENABLED START*/
    void generateSpeechActs(int numberOfTasks);
    const srg::world::Coordinate getRandomCoordinate();
    int sentCounter;
    std::vector<srg::agent::SpeechAct> speechActs;
    /*PROTECTED REGION END*/
};
} /* namespace alica */
