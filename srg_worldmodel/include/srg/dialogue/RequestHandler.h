#pragma once

#include <srg/conceptnet/ConceptNet.h>
#include <srg/agent/containers/SpeechAct.h>

#include <memory>
#include <string>
#include <map>

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
    bool queryCachedResults(const std::string& objectType, const std::string& locationType, std::vector<srg::world::RoomType>& roomTypes);
    std::vector<world::RoomType> getLocations(const std::string& objectType, const std::string& locationType);
    std::shared_ptr<agent::SpeechAct> testGetLocationsViaRelatedness(const agent::SpeechAct requestAct);
    std::string trim(const std::string& s);

    srg::SRGWorldModel* wm;
    srg::conceptnet::ConceptNet* cn;

    std::map<std::pair<std::string,std::string>,std::vector<world::RoomType>> cachedRequests;
};
} // namespace dialogue
} // namespace srg
