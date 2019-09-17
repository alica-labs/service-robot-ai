#pragma once

#include <srgsim/Command.capnp.h>
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

class Robot
{

public:
    static Robot* getInstance(); /**< Singleton Getter */
    virtual ~Robot();

    // methods for doing something
    void spawn() const;

private:
    Robot(srg::SRGWorldModel* wm);  /**< Private Singleton Constructor */
    essentials::SystemConfig* sc;

    std::string simCmdTopic;
    std::string simAddress;

    capnzero::Publisher* simPub;
    void* capnzeroContext;
    essentials::IdentifierConstPtr id;

    srg::SRGWorldModel* wm;
};
} // namespace srg