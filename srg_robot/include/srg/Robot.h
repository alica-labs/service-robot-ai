#pragma once

#include <srgsim/containers/SimCommand.h>
#include <srgsim/containers/Coordinate.h>
#include <srg/dialogue/ManipulationTask.h>
#include <essentials/IdentifierConstPtr.h>

#include <string>

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
    bool move(srgsim::Coordinate goal) const;
    void manipulate(const srg::dialogue::ManipulationTask* task) const;

private:
    Robot(srg::SRGWorldModel* wm);  /**< Private Singleton Constructor */
    void send(srgsim::SimCommand sc) const;

    essentials::SystemConfig* sc;
    std::string simCmdTopic;
    std::string simAddress;
    capnzero::Publisher* simPub;
    void* capnzeroContext;

    srg::SRGWorldModel* wm;
    essentials::IdentifierConstPtr id;
    srg::robot::Movement* movement;

};
} // namespace srg