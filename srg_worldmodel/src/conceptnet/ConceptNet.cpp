#include "srg/conceptnet/ConceptNet.h"
#include "srg/SRGWorldModel.h"
#include "srg/conceptnet/CNManager.h"
#include "srg/conceptnet/Concept.h"
#include "srg/conceptnet/Edge.h"
#include "srg/dialogue/AnswerGraph.h"

#include <curl/curl.h>

#include <ctype.h>

namespace srg
{
namespace conceptnet
{

/*Use this for the web hosted conceptnet*/
const std::string ConceptNet::BASE_URL = "http://api.conceptnet.io";

/*Use this for the locally hosted conceptnet*/
//const std::string ConceptNet::BASE_URL = "http://api.localhost:8084";

const std::string ConceptNet::QUERYSTART = "/query?start=/c/en/";
const std::string ConceptNet::QUERYEND = "/query?end=/c/en/";
const std::string ConceptNet::QUERYNODE = "/query?node=/c/en/";
const std::string ConceptNet::QUERYOTHER = "/query?other=/c/en/";
const std::string ConceptNet::WILDCARD = "wildcard";
const std::string ConceptNet::LIMIT = "&limit=1000";
const std::string ConceptNet::RELATION = "&rel=/r/";
const std::string ConceptNet::END = "&end=/c/en/";
const std::string ConceptNet::RELATEDNESS = "/relatedness?";
const std::string ConceptNet::NODE1 = "node1=/c/en/";
const std::string ConceptNet::NODE2 = "&node2=/c/en/";
const int ConceptNet::SYNONYMDEPTH = 1;

ConceptNet::ConceptNet(SRGWorldModel* wm)
{
    this->wm = wm;
}

Concept* ConceptNet::getConcept(CNManager* cnManager, const std::string& conceptName)
{
    std::string json = httpGet(ConceptNet::BASE_URL + ConceptNet::QUERYNODE + conceptName + "&limit=" + std::to_string(1));
    std::cout << "[RESULT] " << json << std::endl;
    YAML::Node node;
    node = YAML::Load(json);
    if (!isValid(node)) {
        return nullptr;
    }
    std::string conceptId = node["edges"][0]["start"]["term"].as<std::string>();
    if (conceptId.find("/c/en") == std::string::npos || conceptId.find(conceptName) == std::string::npos) {
        conceptId = node["edges"][0]["end"]["term"].as<std::string>();
    }
    return cnManager->createConcept(conceptId, this->trimTerm(conceptId), "");
}

std::vector<Edge*> ConceptNet::getEdges(CNManager* cnManager, const std::string& concept, int limit)
{
    std::vector<Edge*> edges;
    std::string json = httpGet(ConceptNet::BASE_URL + ConceptNet::QUERYNODE + concept + ConceptNet::LIMIT);
    YAML::Node node;
    node = YAML::Load(json);
    if (!isValid(node)) {
        return edges;
    }
    std::string nextPage = generateEdges(cnManager, json, edges, limit);
    while (!nextPage.empty()) {
        json = httpGet(ConceptNet::BASE_URL + nextPage);
        nextPage = generateEdges(cnManager, json, edges, limit);
    }
    return edges;
}

std::vector<Edge*> ConceptNet::getEdges(CNManager* cnManager, Relation relation, const std::string& concept, int limit)
{
    std::vector<Edge*> edges;
    std::string json = httpGet(ConceptNet::BASE_URL + ConceptNet::QUERYNODE + concept + ConceptNet::RELATION + relations[relation] + ConceptNet::LIMIT);
    std::string nextPage = generateEdges(cnManager, json, edges, limit);
    while (!nextPage.empty()) {
        json = httpGet(ConceptNet::BASE_URL + nextPage);
        nextPage = generateEdges(cnManager, json, edges, limit);
    }
    return edges;
}

std::vector<Edge*> ConceptNet::getCompleteEdges(
        CNManager* cnManager, Relation relation, const std::string& fromConcept, const std::string& toConcept, int limit)
{
    std::vector<Edge*> edges;
    std::string json = httpGet(
            ConceptNet::BASE_URL + ConceptNet::QUERYSTART + fromConcept + ConceptNet::RELATION + relations[relation] + END + toConcept + ConceptNet::LIMIT);
    std::string nextPage = generateEdges(cnManager, json, edges, limit);
    while (!nextPage.empty()) {
        json = httpGet(ConceptNet::BASE_URL + nextPage);
        nextPage = generateEdges(cnManager, json, edges, limit);
    }
    return edges;
}
std::vector<Edge*> ConceptNet::getOutgoingEdges(CNManager* cnManager, Relation relation, const std::string& fromConcept, int limit)
{
    std::vector<Edge*> edges;
    std::string json = httpGet(ConceptNet::BASE_URL + ConceptNet::QUERYSTART + fromConcept + ConceptNet::RELATION + relations[relation] + ConceptNet::LIMIT);
    std::string nextPage = generateEdges(cnManager, json, edges, limit);
    while (!nextPage.empty()) {
        json = httpGet(ConceptNet::BASE_URL + nextPage);
        nextPage = generateEdges(cnManager, json, edges, limit);
    }
    return edges;
}
std::vector<Edge*> ConceptNet::getIncomingEdges(CNManager* cnManager, Relation relation, const std::string& toConcept, int limit)
{
    std::vector<Edge*> edges;
    std::string json = httpGet(ConceptNet::BASE_URL + ConceptNet::QUERYEND + toConcept + ConceptNet::RELATION + relations[relation] + ConceptNet::LIMIT);
    std::string nextPage = generateEdges(cnManager, json, edges, limit);
    while (!nextPage.empty()) {
        json = httpGet(ConceptNet::BASE_URL + nextPage);
        nextPage = generateEdges(cnManager, json, edges, limit);
    }
    return edges;
}
std::vector<Edge*> ConceptNet::getRelations(CNManager* cnManager, const std::string& concept, const std::string& otherConcept, int limit)
{
    std::vector<Edge*> edges;
    std::string json = httpGet(ConceptNet::BASE_URL + ConceptNet::QUERYSTART + concept + ConceptNet::END + otherConcept + ConceptNet::LIMIT);
    std::string nextPage = generateEdges(cnManager, json, edges, limit);
    while (!nextPage.empty()) {
        json = httpGet(ConceptNet::BASE_URL + nextPage);
        nextPage = generateEdges(cnManager, json, edges, limit);
    }
    return edges;
}

std::vector<Edge*> ConceptNet::getEquivalentOutgoingEdges(CNManager* cnManager, const conceptnet::Concept* concept, int limit)
{
    std::vector<Edge*> edges = this->getOutgoingEdges(cnManager, conceptnet::Synonym, concept->term, limit);

    std::vector<Edge*> similarToEdges = this->getOutgoingEdges(cnManager, conceptnet::SimilarTo, concept->term, limit);
    edges.insert(edges.end(), similarToEdges.begin(), similarToEdges.end());

    std::vector<Edge*> instanceOfEdges = this->getOutgoingEdges(cnManager, conceptnet::InstanceOf, concept->term, limit);
    edges.insert(edges.end(), instanceOfEdges.begin(), instanceOfEdges.end());

    return edges;
}

std::vector<Concept*> ConceptNet::getEquivalentConcepts(CNManager* cnManager, conceptnet::Concept* concept, int limit)
{
    std::vector<conceptnet::Edge*> edges = this->getEquivalentOutgoingEdges(cnManager, concept, -1);
    std::vector<conceptnet::Concept*> equivalentConcepts;
    for (conceptnet::Edge* edge : edges) {
        equivalentConcepts.push_back(edge->toConcept);
    }
    return equivalentConcepts;
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

    std::cout << "[Result] " << *httpData << std::endl;
    return *httpData;

}

std::string ConceptNet::generateEdges(CNManager* cnManager, const std::string& json, std::vector<Edge*>& edges, int limit, double minWeight)
{
    YAML::Node node;
    node = YAML::Load(json);
    if (!isValid(node)) {
        return "";
    }
    YAML::Node jsonEdges = node["edges"];
    for (size_t i = 0; i < jsonEdges.size(); i++) {
        YAML::Node edge = jsonEdges[i];
        double weight = edge["weight"].as<double>();
        if (weight < minWeight) {
            std::cout << "ConceptNet: below min weight" << std::endl;
            return "";
        }
        // end of edge
        YAML::Node end = edge["end"];
        if (!end["language"]) {
            continue;
        }
        std::string endLanguage = end["language"].as<std::string>();
        // skip non English
        if (endLanguage != "en") {
            continue;
        }
        std::string endTerm = end["term"].as<std::string>();
        if (this->conceptContainsForbiddenCharacter(endTerm)) {
            std::cout << "ConceptNet: Skipping Concept:" << endTerm << std::endl;
            continue;
        }
        std::string endSenseLabel = "";
        if (end["sense_label"].IsDefined()) {
            endSenseLabel = end["sense_label"].as<std::string>();
        }
        std::string endID = end["@id"].as<std::string>();
        // start of edge
        YAML::Node start = edge["start"];
        if (!start["language"]) {
            continue;
        }
        std::string startLanguage = start["language"].as<std::string>();
        // skip non English
        if (startLanguage != "en") {
            continue;
        }
        std::string startTerm = start["term"].as<std::string>();
        if (this->conceptContainsForbiddenCharacter(startTerm)) {
            std::cout << "ConceptNet: Skipping concept:" << startTerm << std::endl;
            continue;
        }
        std::string startSenseLabel = "";
        if (start["sense_label"].IsDefined()) {
            startSenseLabel = start["sense_label"].as<std::string>();
        }
        std::string startID = start["@id"].as<std::string>();
        std::string relation = edge["rel"]["@id"].as<std::string>();
        relation = trimTerm(relation);
        // create edge
        std::string trimmedStartTerm = trimTerm(startTerm);
        std::string trimmedEndTerm = trimTerm(endTerm);
        if (std::isdigit(trimmedStartTerm[0])) {
            std::cout << "ConceptNet: Skipping concept:" << startTerm << std::endl;
            continue;
        }
        if (std::isdigit(trimmedEndTerm[0])) {
            std::cout << "ConceptNet: Skipping concept:" << endTerm << std::endl;
            continue;
        }

        Concept* fromConcept = cnManager->createConcept(startTerm, trimmedStartTerm, startSenseLabel);
        Concept* toConcept = cnManager->createConcept(endTerm, trimmedEndTerm, endSenseLabel);
        std::string edgeId = "/a[" + relation + "," + startTerm + "," + endTerm + "]"; // dont use cn's edgeId, its evil!
        edges.push_back(cnManager->createEdge(edgeId, startLanguage, fromConcept, toConcept, getRelation(relation), weight));
        if (limit != -1 && edges.size() == limit) {
            break;
        }
    }
    if (!node["view"].IsDefined() && !node["view"]["nextPage"].IsDefined()) {
        return "";
    }
    if (limit == -1) {
        std::cout << "ConceptNet: nextPage: " << node["view"]["nextPage"].as<std::string>() << std::endl;
        return node["view"]["nextPage"].as<std::string>();
    } else {
        return "";
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

bool ConceptNet::conceptContainsForbiddenCharacter(const std::string& concept)
{
    for (size_t i = 0; i < concept.length(); i++) {
        if (!isascii(concept.at(i))) {
            return true;
        }
    }
    if (concept.find_first_of(":!@") != std::string::npos) {
        return true;
    }
    if (trimTerm(concept) == "not") {
        return true;
    }
    return false;
}

std::string ConceptNet::trimTerm(const std::string& term)
{
    auto pos = term.find_last_of('/');
    return term.substr(pos + 1, term.length() - pos - 1);
}

void ConceptNet::findInconsistencies(srg::dialogue::AnswerGraph* answerGraph, int limit)
{
    collectAntonyms(answerGraph, limit);
    std::vector<Concept*> newAntonyms;
    for (int i = 0; i < ConceptNet::SYNONYMDEPTH; i++) {
        newAntonyms = getNewAdjectives(answerGraph);
        for (auto concept : newAntonyms) {
            std::vector<Edge*> equivalent = this->getEquivalentOutgoingEdges(answerGraph, concept, limit);
            if (answerGraph->adjectiveAntonymMap.find(concept) == answerGraph->adjectiveAntonymMap.end()) {
                answerGraph->adjectiveAntonymMap.emplace(concept, equivalent);
            } else {
                for (Edge* edge : equivalent) {
                    auto tmp = answerGraph->adjectiveAntonymMap.at(concept);
                    if (std::find(tmp.begin(), tmp.end(), edge) == tmp.end()) {
                        answerGraph->adjectiveAntonymMap.at(concept).push_back(edge);
                    }
                }
            }
            concept->addEdges(equivalent);
        }
    }

    for (auto pair : answerGraph->adjectiveAntonymMap) {
        if (pair.second.empty()) {
            continue;
        }
        std::cout << "ConceptNet: Concept: " << pair.first->term << std::endl;
        for (auto edge : pair.second) {
            std::cout << "\tConceptNet Antonym edge: " << edge->toString() << std::endl;
        }
    }
    std::cout << "################# map size: " << answerGraph->adjectiveAntonymMap.size() << std::endl;
}

std::vector<Concept*> ConceptNet::getNewAdjectives(srg::dialogue::AnswerGraph* answerGraph)
{
    std::vector<Concept*> ret;
    for (auto pair : answerGraph->adjectiveAntonymMap) {
        for (srg::conceptnet::Edge* edge : pair.second) {
            if (answerGraph->closedProperties.find(edge->fromConcept) == answerGraph->closedProperties.end()) {
                if (std::find(ret.begin(), ret.end(), edge->fromConcept) == ret.end()) {
                    ret.push_back(edge->fromConcept);
                    answerGraph->closedProperties.insert(edge->fromConcept);
                }
            }
            if (answerGraph->closedProperties.find(edge->toConcept) == answerGraph->closedProperties.end()) {
                if (std::find(ret.begin(), ret.end(), edge->toConcept) == ret.end()) {
                    ret.push_back(edge->toConcept);
                    answerGraph->closedProperties.insert(edge->toConcept);
                }
            }
        }
    }
    return ret;
}

void ConceptNet::collectAntonyms(srg::dialogue::AnswerGraph* answerGraph, int limit)
{
    for (auto pair : answerGraph->getEdges()) {
        if (pair.second->relation != srg::conceptnet::Relation::HasProperty) {
            continue;
        }

        srg::conceptnet::Concept* concept;
        if (pair.second->fromConcept == answerGraph->root) {
            concept = pair.second->toConcept;
        } else {
            concept = pair.second->fromConcept;
        }

        std::vector<Edge*> edges = this->getEdges(answerGraph, Relation::Antonym, concept->term, limit);
        std::cout << "collectAntonyms: " << concept->term << std::endl;
        answerGraph->adjectiveAntonymMap.emplace(concept, edges);
        concept->addEdges(edges);
    }
}
} // namespace conceptnet

} // namespace srg
