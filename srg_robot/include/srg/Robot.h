#pragma once

#include <srg/sim/containers/SimCommand.h>
#include <srg/world/Coordinate.h>
#include <essentials/IdentifierConstPtr.h>

#include <string>
#include <srg/robot/ObjectSearch.h>

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
namespace robot {
    class Movement;
}

class Robot
{

public:
    static Robot* getInstance(); /**< Singleton Getter */
    virtual ~Robot();

    // methods for doing something
    void spawn() const;
    bool move(srg::world::Coordinate goal) const;
    void manipulate(const srg::tasks::Task* task) const;
    int32_t getPathCost(srg::world::Coordinate goal) const;

private:
    Robot(srg::SRGWorldModel* wm);  /**< Private Singleton Constructor */
    void send(srg::sim::containers::SimCommand sc) const;

    essentials::SystemConfig* sc;
    std::string simCmdTopic;
    std::string simAddress;
    capnzero::Publisher* simPub;
    void* capnzeroContext;

    srg::SRGWorldModel* wm;
    essentials::IdentifierConstPtr id;
    srg::robot::Movement* movement;
    srg::robot::ObjectSearch* search;

};
} // namespace srg