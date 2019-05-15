#pragma once

#include <engine/AlicaClock.h>
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

class SRGWorldModel;

namespace wm
{
class SRGSimData
{
public:
    SRGSimData(SRGWorldModel* wm);
    virtual ~SRGSimData();

    // methods for processing messages from SRG Sim
 

    // data access through public buffers
   

private:
    SRGWorldModel* wm;
    essentials::SystemConfig* sc;
};
} /* namespace wm */
} /* namespace srg */
