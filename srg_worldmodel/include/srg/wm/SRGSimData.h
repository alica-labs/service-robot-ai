#pragma once

#include <engine/AlicaClock.h>
#include <srgsim/containers/SimPerceptions.h>
#include <supplementary/InfoBuffer.h>
#include <supplementary/InformationElement.h>

#include <vector>
namespace essentials
{
class SystemConfig;
}

namespace srgsim
{
class World;
class Coordinate;
} // namespace srgsim

namespace alica
{
class AlicaTime;
}
namespace srg
{
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

    const supplementary::InfoBuffer<srgsim::Coordinate>& getOwnPositionBuffer() const;

    void processPerception(srgsim::SimPerceptions simPerceptions);
    bool isLocalised();
    const srgsim::World* getWorld();

private:
    SRGWorldModel* wm;
    essentials::SystemConfig* sc;
    srgsim::World* world;
    dialogue::DialogueManager* dialogueManager;

    alica::AlicaTime ownPositionValidityDuration;
    supplementary::InfoBuffer<srgsim::Coordinate>* ownPositionBuffer;
};
} /* namespace wm */
} /* namespace srg */
