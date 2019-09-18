#pragma once

#include <engine/AlicaClock.h>
#include <supplementary/InfoBuffer.h>
#include <supplementary/InformationElement.h>
#include <srgsim/containers/SimPerceptions.h>

#include <vector>
namespace essentials
{
class SystemConfig;
}

namespace srgsim
{
class World;
}

namespace alica
{
class AlicaTime;
}
namespace srg
{

class SRGWorldModel;

namespace wm
{
class SRGSimData
{
public:
    SRGSimData(SRGWorldModel* wm);
    virtual ~SRGSimData();
    void processPerception(srgsim::SimPerceptions simPerceptions);

private:
    SRGWorldModel* wm;
    essentials::SystemConfig* sc;
    srgsim::World* world;
};
} /* namespace wm */
} /* namespace srg */
