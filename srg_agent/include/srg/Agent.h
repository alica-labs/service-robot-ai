#pragma once

#include <srg/sim/containers/SimCommand.h>
#include <srg/world/Coordinate.h>
#include <essentials/IdentifierConstPtr.h>

#include <string>
#include <srg/agent/ObjectSearch.h>

namespace capnzero
{
class Publisher;
}

namespace essentials
{
class SystemConfig;
}

namespace srg
{

class SRGWorldModel;
namespace tasks {
    class Task;
}
namespace agent {
    class Voice;
    class Movement;
}

class Agent
{

public:
    static Agent* getInstance(); /**< Singleton Getter */
    virtual ~Agent();

    // methods for doing something
    void spawn() const;
    bool move(srg::world::Coordinate goal) const;
    void manipulate(const srg::tasks::Task* task) const;
    void speak(std::string input) const;
    void speak(srg::agent::SpeechAct sa) const;
    int32_t getPathCost(srg::world::Coordinate goal) const;

private:
    Agent(srg::SRGWorldModel* wm);  /**< Private Singleton Constructor */
    void send(srg::sim::containers::SimCommand sc) const;

    essentials::SystemConfig* sc;
    std::string simCmdTopic;
    std::string simAddress;
    capnzero::Publisher* simPub;
    void* capnzeroContext;

    srg::SRGWorldModel* wm;
    essentials::IdentifierConstPtr id;
    srg::agent::Movement* movement;
    srg::agent::ObjectSearch* search;
    srg::agent::Voice* voice;

};
} // namespace srg