#include "srg/wm/ConceptNet.h"
#include "srg/SRGWorldModel.h"

namespace srg
{
namespace wm
{

const std::string ConceptNet::BASE_URL = "http://api.localhost:8084";
const std::string ConceptNet::URL_QUERYSTART = "/query?start=/c/en/";
const std::string ConceptNet::URL_QUERYEND = "/query?end=/c/en/";
const std::string ConceptNet::URL_QUERYNODE = "/query?node=/c/en/";
const std::string ConceptNet::URL_QUERYOTHER = "/query?other=/c/en/";
const std::string ConceptNet::PREFIX = "cn5_";
const std::string ConceptNet::WILDCARD = "wildcard";

ConceptNet::ConceptNet(SRGWorldModel* wm)
{
    this->wm = wm;
}

ConceptNet::~ConceptNet() {}

std::string ConceptNet::getConcept(std::string concept) {
    return nullptr;
}
std::string ConceptNet::getCompleteEdge(std::string relation, std::string fromConcept, std::string toConcept) {
    return nullptr;
}
std::string ConceptNet::getOutgoingEdges(std::string relation, std::string fromConcept) {
    return nullptr;
}
std::string ConceptNet::getIncomingEdges(std::string relation, std::string toConcept) {
    return nullptr;
}
double ConceptNet::getRelatedness(std::string firstConcept, std::string secondConcept) {
    return 0.0;
}

} // namespace wm

} // namespace srg
