#pragma once

#include <engine/AlicaClock.h>
#include <srg/sim/containers/Perceptions.h>
#include <supplementary/InfoBuffer.h>
#include <supplementary/InformationElement.h>

#include <vector>
namespace essentials
{
class SystemConfig;
}

namespace alica
{
class AlicaTime;
}
namespace srg
{
class World;
namespace world {
    class Coordinate;
}
namespace dialogue
{
class DialogueManager;
}
class SRGWorldModel;

namespace wm
{
class SRGSimData
{
public:
    SRGSimData(SRGWorldModel* wm);
    virtual ~SRGSimData();

    void init();
    const supplementary::InfoBuffer<srg::world::Coordinate>& getOwnPositionBuffer() const;

    void processPerception(srg::sim::containers::Perceptions perceptions);
    bool isLocalised();
    srg::World* getWorld();

private:
    SRGWorldModel* wm;
    essentials::SystemConfig& sc;
    srg::World* world;
    dialogue::DialogueManager* dialogueManager;

    alica::AlicaTime ownPositionValidityDuration;
    supplementary::InfoBuffer<srg::world::Coordinate>* ownPositionBuffer;
};
} /* namespace wm */
} /* namespace srg */
