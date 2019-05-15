#include "srg/wm/SRGSimData.h"

#include "srg/SRGWorldModel.h"
#include "srg/wm/ASPKnowledgeBase.h"

#include <SystemConfig.h>
#include <engine/AlicaClock.h>

#include <exception>
#include <iostream>
#include <map>
#include <memory>
#include <utility>

using std::make_shared;
using std::shared_ptr;
using supplementary::InfoBuffer;
using supplementary::InformationElement;

namespace srg
{
namespace wm
{

SRGSimData::SRGSimData(SRGWorldModel* wm)
{
    this->wm = wm;
    this->sc = this->wm->getSystemConfig();

    // data buffers

}

SRGSimData::~SRGSimData() {}

} // namespace wm
} /* namespace srg */
