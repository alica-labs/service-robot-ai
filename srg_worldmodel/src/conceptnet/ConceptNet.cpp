#include "srg/conceptnet/ConceptNet.h"
#include "srg/SRGWorldModel.h"
#include "srg/conceptnet/Concept.h"
#include "srg/conceptnet/CNManager.h"

#include <curl/curl.h>

#include <ctype.h>

namespace srg
{
namespace conceptnet
{

/*Use this for the web hosted conceptnet*/
//const std::string ConceptNet::BASE_URL = "http://api.conceptnet.io";

/*Use this for the locally hosted conceptnet*/
const std::string ConceptNet::BASE_URL = "http://api.localhost:8084";

const std::string ConceptNet::QUERYSTART = "/query?start=/c/en/";
const std::string ConceptNet::QUERYEND = "/query?end=/c/en/";
const std::string ConceptNet::QUERYNODE = "/query?node=/c/en/";
const std::string ConceptNet::QUERYOTHER = "/query?other=/c/en/";
const std::string ConceptNet::WILDCARD = "wildcard";
const std::string ConceptNet::LIMIT = "&limit=";
const std::string ConceptNet::RELATION = "&rel=/r/";
const std::string ConceptNet::END = "&end=/c/en/";
const std::string ConceptNet::RELATEDNESS = "/relatedness?";
const std::string ConceptNet::NODE1 = "node1=/c/en/";
const std::string ConceptNet::NODE2 = "&node2=/c/en/";

ConceptNet::ConceptNet(SRGWorldModel* wm)
{
    this->wm = wm;
}

Concept* ConceptNet::getConcept(CNManager* cnManager, const std::string& conceptName)
{
    std::string json = httpGet(ConceptNet::BASE_URL + ConceptNet::QUERYNODE + conceptName + ConceptNet::LIMIT + std::to_string(1));
    YAML::Node node;
    node = YAML::Load(json);
    if (!isValid(node)) {
        return nullptr;
    }
    std::string conceptId = node["edges"][0]["start"]["term"].as<std::string>();
    if(conceptId.find(conceptName) == std::string::npos) {
        conceptId = node["edges"][0]["end"]["term"].as<std::string>();
    }
    return cnManager->createConcept(conceptId, this->trimTerm(conceptId), "");
}

std::vector<Edge*> ConceptNet::getEdges(CNManager* cnManager, const std::string& concept, int limit)
{
    std::vector<Edge*> edges;
    std::string json = httpGet(ConceptNet::BASE_URL + ConceptNet::QUERYNODE + concept + ConceptNet::LIMIT + std::to_string(limit));
    generateEdges(cnManager, json, edges);
    return edges;
}

std::vector<Edge*> ConceptNet::getEdges(CNManager* cnManager, Relation relation, const std::string& concept, int limit) {
    std::vector<Edge*> edges;
    std::string json = httpGet(ConceptNet::BASE_URL + ConceptNet::QUERYNODE + concept + ConceptNet::RELATION + relations[relation] + ConceptNet::LIMIT + std::to_string(limit));
    generateEdges(cnManager, json, edges);
    return edges;
}

std::vector<Edge*> ConceptNet::getCompleteEdge(CNManager* cnManager, Relation relation, const std::string& fromConcept, const std::string& toConcept, int limit)
{
    std::vector<Edge*> edges;
    std::string json = httpGet(ConceptNet::BASE_URL + ConceptNet::QUERYSTART + fromConcept + ConceptNet::RELATION + relations[relation] + END +
                               toConcept + ConceptNet::LIMIT + std::to_string(limit));
    generateEdges(cnManager, json, edges);
    return edges;
}
std::vector<Edge*> ConceptNet::getOutgoingEdges(CNManager* cnManager, Relation relation, const std::string& fromConcept, int limit)
{
    std::vector<Edge*> edges;
    std::string json = httpGet(
            ConceptNet::BASE_URL + ConceptNet::QUERYSTART + fromConcept + ConceptNet::RELATION + relations[relation] + ConceptNet::LIMIT + std::to_string(limit));
    generateEdges(cnManager, json, edges);
    return edges;
}
std::vector<Edge*> ConceptNet::getIncomingEdges(CNManager* cnManager, Relation relation, const std::string& toConcept, int limit)
{
    std::vector<Edge*> edges;
    std::string json =
            httpGet(ConceptNet::BASE_URL + ConceptNet::QUERYEND + toConcept + ConceptNet::RELATION + relations[relation] + ConceptNet::LIMIT + std::to_string(limit));
    generateEdges(cnManager, json, edges);
    return edges;
}
std::vector<Edge*> ConceptNet::getRelations(CNManager* cnManager, const std::string& concept, const std::string& otherConcept, int limit)
{
    std::vector<Edge*> edges;
    std::string json = httpGet(ConceptNet::BASE_URL + ConceptNet::QUERYSTART + concept + ConceptNet::END + otherConcept + ConceptNet::LIMIT + std::to_string(limit));
    generateEdges(cnManager, json, edges);
    return edges;
}

std::vector<Edge*> ConceptNet::getEquivalentOutgoingEdges(CNManager* cnManager, const conceptnet::Concept* concept, int limit) {
    std::vector<Edge*> edges = this->getOutgoingEdges(cnManager, conceptnet::Synonym, concept->term, limit);

    std::vector<Edge*> simirToEdges = this->getOutgoingEdges(cnManager, conceptnet::SimilarTo, concept->term, limit);
    edges.insert(edges.end(), simirToEdges.begin(), simirToEdges.end());

    std::vector<Edge*> instanceOfEdges = this->getOutgoingEdges(cnManager, conceptnet::InstanceOf, concept->term, limit);
    edges.insert(edges.end(), instanceOfEdges.begin(), instanceOfEdges.end());

    return edges;
}

double ConceptNet::getRelatedness(const std::string& firstConcept, const std::string& secondConcept)
{
    std::string json = httpGet(ConceptNet::BASE_URL + ConceptNet::RELATEDNESS + ConceptNet::NODE1 + firstConcept + ConceptNet::NODE2 + secondConcept);
    YAML::Node node;
    node = YAML::Load(json);
    if (!isValid(node)) {
        return NAN;
    }
    return node["value"].as<double>();
}

std::size_t callback(const char* in, std::size_t size, std::size_t num, std::string* out)
{
    const std::size_t totalBytes(size * num);
    out->append(in, totalBytes);
    return totalBytes;
}

std::string ConceptNet::httpGet(const std::string& url)
{
    CURL* curl = curl_easy_init();

    // Set remote URL.
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Don't bother trying IPv6, which would increase DNS resolution time.
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

    // Don't wait forever, time out after 10 seconds.
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

    // Follow HTTP redirects if necessary.
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // Response information.
    int httpCode(0);
    std::string* tmp = new std::string();
    std::unique_ptr<std::string> httpData(move(tmp));

    // Hook up data handling function.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);

    // Hook up data container (will be passed as the last parameter to the
    // callback handling function).  Can be any pointer type, since it will
    // internally be passed as a void pointer.
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

    // Run our HTTP GET command, capture the HTTP response code, and clean up.
    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    return *httpData;
}

void ConceptNet::generateEdges(CNManager* cnManager, const std::string& json, std::vector<Edge*>& edges, double minWeight)
{
    YAML::Node node;
    node = YAML::Load(json);
    if (!isValid(node)) {
        return;
    }
    YAML::Node jsonEdges = node["edges"];
    for (size_t i = 0; i < jsonEdges.size(); i++) {
        YAML::Node edge = jsonEdges[i];
        double weight = edge["weight"].as<double>();
        if(weight < minWeight) {
            return;
        }
        // end of edge
        YAML::Node end = edge["end"];
        std::string endLanguage = end["language"].as<std::string>();
        // skip non English
        if (endLanguage != "en") {
            continue;
        }
        std::string endTerm = end["term"].as<std::string>();
        if (std::isdigit(endTerm.at(0)) || this->conceptContainsNonASCII(endTerm)) {
            std::cout << "ConceptNetQueryCommand: Skipping Concept:" << endTerm << std::endl;
            continue;
        }
        std::string endSenseLabel = "";
        if (end["sense_label"].IsDefined()) {
            endSenseLabel = end["sense_label"].as<std::string>();
        }
        std::string endID = end["@id"].as<std::string>();
        // start of edge
        YAML::Node start = edge["start"];
        std::string startLanguage = start["language"].as<std::string>();
        // skip non English
        if (startLanguage != "en") {
            continue;
        }
        std::string startTerm = start["term"].as<std::string>();
        if (std::isdigit(startTerm.at(0)) || this->conceptContainsNonASCII(startTerm)) {
            std::cout << "ConceptNetQueryCommand: Skipping concept:" << startTerm << std::endl;
            continue;
        }
        std::string startSenseLabel = "";
        if (start["sense_label"]) {
            startSenseLabel = start["sense_label"].as<std::string>();
        }
        std::string startID = start["@id"].as<std::string>();
        std::string relation = edge["rel"]["@id"].as<std::string>();
        relation = trimTerm(relation); //.right(relation.size() - relation.lastIndexOf('/') - 1);
        // create edge
        Concept* fromConcept = cnManager->createConcept(startTerm, trimTerm(startTerm), startSenseLabel);
        Concept* toConcept = cnManager->createConcept(endTerm, trimTerm(endTerm), endSenseLabel);
        std::string edgeId = "/a["+relation+","+startTerm+","+endTerm+"]"; // dont use cn's edgeId, its evil!
        edges.push_back(cnManager->createEdge(edgeId, startLanguage, fromConcept, toConcept, getRelation(relation), weight));
//        edges.push_back(cnManager->createEdge(edgeId, startLanguage, fromConcept, toConcept, getRelation(relation), weight * edge["sources"].size()));
    }
}

bool ConceptNet::isValid(const YAML::Node& node)
{
    return node && YAML::NodeType::Null != node.Type();
}

Relation ConceptNet::getRelation(const std::string& relation)
{
    int position = 0;
    for (auto& rel : relations) {
        std::string str(rel);
        if (str == relation) {
            return static_cast<Relation>(position);
        }
        position++;
    }
    return Relation::UNDEFINED;
}

bool ConceptNet::conceptContainsNonASCII(const std::string& concept)
{
    for (size_t i = 0; i < concept.length(); i++) {
        if (!isascii(concept.at(i))) {
            return true;
        }
    }
    return false;
}

std::string ConceptNet::trimTerm(const std::string& term)
{
    auto pos = term.find_last_of('/');
    return term.substr(pos + 1, term.length() - pos - 1);
}

} // namespace wm

} // namespace srg
