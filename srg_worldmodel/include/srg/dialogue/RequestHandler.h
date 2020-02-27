#pragma once

#include <srg/conceptnet/ConceptNet.h>
#include <srg/agent/containers/SpeechAct.h>

#include <memory>
#include <string>

namespace srg
{
class SRGWorldModel;
namespace dialogue
{
class BasicHumanNeeds;
class RequestHandler
{
public:
    explicit RequestHandler(SRGWorldModel* wm);

    std::shared_ptr<agent::SpeechAct> handle(const agent::SpeechAct requestAct);

private:
    std::shared_ptr<agent::SpeechAct> getLocations(const std::string& objectType, const std::string& locationType);
    std::shared_ptr<agent::SpeechAct> testGetLocationsViaRelatedness(const agent::SpeechAct requestAct);

    srg::SRGWorldModel* wm;
    srg::conceptnet::ConceptNet* cn;
};
} // namespace dialogue
} // namespace srg
