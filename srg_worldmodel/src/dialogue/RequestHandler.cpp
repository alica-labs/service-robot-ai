#include "srg/dialogue/RequestHandler.h"

#include "srg/SRGWorldModel.h"
#include "srg/asp/ASPTranslator.h"
#include "srg/asp/SRGKnowledgeManager.h"
#include "srg/conceptnet/Concept.h"
#include "srg/conceptnet/ConceptPath.h"
#include "srg/dialogue/AnswerGraph.h"
#include "srg/dialogue/BasicHumanNeeds.h"

#include <engine/AlicaEngine.h>

namespace srg
{
namespace dialogue
{
RequestHandler::RequestHandler(SRGWorldModel* wm)
        : wm(wm)
{
    this->cn = this->wm->conceptNet;
}

std::shared_ptr<agent::SpeechAct> RequestHandler::handle(const agent::SpeechAct requestAct)
{
    // Handling of Basic Human Needs
    if (requestAct.text.find("known-locations") == 0) {
        // TODO: Answer requests for known objects
        std::cout << "[RequestHandler] Was queried for known locations: " << requestAct << std::endl;
        return nullptr;
    }

    // Query Concept Net for a Concept's Location and integrate that knowledge into the ASP Knowledge Base
    std::string objectTypeString;
    switch (requestAct.objectRequestType) {
    case srg::world::ObjectType::CupYellow:
    case srg::world::ObjectType::CupBlue:
    case srg::world::ObjectType::CupRed:
        objectTypeString = "cup";
        break;
    default:
        objectTypeString = "unknown";
    }
    std::vector<world::RoomType> roomTypes = getLocations(objectTypeString, requestAct.text);

    // prepare answer
    std::shared_ptr<agent::SpeechAct> answerSpeechAct = std::make_shared<agent::SpeechAct>();
    answerSpeechAct->type = agent::SpeechType::request;
    answerSpeechAct->actID = this->wm->getEngine()->getIdManager()->generateID();
    answerSpeechAct->senderID = this->wm->getOwnId();
    answerSpeechAct->previousActID = requestAct.actID;
    answerSpeechAct->probableRoomTypes = roomTypes;
    return answerSpeechAct;
}

std::vector<world::RoomType> RequestHandler::getLocations(const std::string& objectType, const std::string& locationType)
{
    std::vector<world::RoomType> roomTypes;
    if (this->queryCachedResults(objectType, locationType, roomTypes)) {
        return roomTypes;
    }

    srg::dialogue::AnswerGraph* constructionGraph = new srg::dialogue::AnswerGraph();

    constructionGraph->root = this->cn->getConcept(constructionGraph, objectType); // << should be SearchedItem
    if (!constructionGraph->root) {
        // did not find any concept for the given object type
        std::cout << "[InformHandler] No root concept found for '" << objectType << "'" << std::endl;
        return std::vector<world::RoomType>();
    }

    // init fringe
    std::vector<conceptnet::ConceptPath*> fringe;
    conceptnet::ConceptPath* path = new conceptnet::ConceptPath(constructionGraph->root); // SearchedItem
    fringe.push_back(path);
    for (conceptnet::Edge* edge : this->cn->getEquivalentOutgoingEdges(constructionGraph, constructionGraph->root, -1)) {
        conceptnet::ConceptPath* path = new conceptnet::ConceptPath(constructionGraph->root);
        path->addToPath(edge); // things similar to SearchedItem
        fringe.push_back(path);
    }

    while (!fringe.empty()) {
        conceptnet::ConceptPath* currentPath = fringe[0];
        fringe.erase(fringe.begin());
        //        std::cout << "[InformHandler] CurrentPath: " << std::endl << currentPath->toString("\t") << std::endl;

        // GOAL CHECK
        std::vector<conceptnet::Edge*> locationTypeCheckEdges =
                this->cn->getCompleteEdges(constructionGraph, conceptnet::Relation::IsA, currentPath->getEnd()->term, locationType, -1);
        if (!locationTypeCheckEdges.empty()) {
            constructionGraph->answerPaths.push_back(currentPath);
            if (std::find(constructionGraph->answerConcepts.begin(), constructionGraph->answerConcepts.end(), currentPath->getEnd()) ==
                    constructionGraph->answerConcepts.end()) {
                constructionGraph->answerConcepts.push_back(currentPath->getEnd());
            }
            continue;
        }

        // LIMIT SEARCH DEPTH
        if (currentPath->getPath().size() == 2) {
            delete currentPath;
            continue;
        }

        // EXPAND SEARCH
        bool expanded = false;
        for (conceptnet::Edge* edge : this->cn->getOutgoingEdges(constructionGraph, conceptnet::Relation::AtLocation, currentPath->getEnd()->term, -1)) {
            if (currentPath->containsNode(edge->toConcept)) {
                continue;
            }
            conceptnet::ConceptPath* nextPath = new conceptnet::ConceptPath(*currentPath);
            nextPath->addToPath(edge);
            fringe.push_back(nextPath);
            expanded = true;
        }
        if (!expanded) {
            delete currentPath;
        }
    }
    constructionGraph->calculateUtilities();

    // build the answer graph we want
    std::vector<srg::conceptnet::Concept*> bestAnswers = constructionGraph->getBestAnswers(3);
    AnswerGraph* answerGraph = new srg::dialogue::AnswerGraph();
    answerGraph->createConcept(constructionGraph->root->id, constructionGraph->root->term, constructionGraph->root->senseLabel);
    answerGraph->root = answerGraph->getConcept(constructionGraph->root->id);
    for (srg::conceptnet::ConceptPath* answerPath : constructionGraph->answerPaths) {
        if (find(bestAnswers.begin(), bestAnswers.end(), answerPath->getEnd()) == bestAnswers.end()) {
            continue;
        }
        answerGraph->createConcept(answerPath->getEnd()->id, answerPath->getEnd()->term, answerPath->getEnd()->senseLabel);
        answerGraph->createEdge(
                constructionGraph->root->term + "_" + conceptnet::relations[conceptnet::Relation::AtLocation] + "_" + answerPath->getEnd()->term, "en",
                answerGraph->getConcept(constructionGraph->root->id), answerGraph->getConcept(answerPath->getEnd()->id), conceptnet::Relation::AtLocation,
                answerPath->getUtility());
    }

    // Integrate Commonsense Knowledge into ASP
    this->wm->aspTranslator->addToKnowledgeBase(answerGraph, asp::ASPTranslator::InconsistencyRemoval::UseExternals);

    // Filter Model for location of objects
    std::vector<std::string> resultStrings = this->wm->srgKnowledgeManager->ask("cs_AtLocation(" + objectType + ", wildcard, wildcard)");

    for (auto& result : resultStrings) {
        size_t firstCommaIdx = result.find(",", 0);
        size_t secondCommaIdx = result.find(",", firstCommaIdx + 1);
        std::string roomTypeString = this->trim(result.substr(firstCommaIdx + 1, secondCommaIdx - firstCommaIdx - 1));
        std::cout << "[RequestHandler] " << roomTypeString << std::endl;
        roomTypes.push_back(srg::world::FromString(roomTypeString));
    }

    this->cachedRequests.insert(std::make_pair(std::make_pair(objectType, locationType), roomTypes));

    return roomTypes;
}

bool RequestHandler::queryCachedResults(const std::string& objectType, const std::string& locationType, std::vector<srg::world::RoomType>& roomTypes) {
    for (auto& cacheEntry : this->cachedRequests) {
        if (cacheEntry.first.first.compare(objectType) == 0
        && cacheEntry.first.second.compare(locationType) == 0) {
            roomTypes = cacheEntry.second;
            return true;
        }
    }
    return false;
}

std::string RequestHandler::trim(const std::string& s)
{
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        start++;
    }

    auto end = s.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return std::string(start, end + 1);
}

std::shared_ptr<agent::SpeechAct> RequestHandler::testGetLocationsViaRelatedness(const agent::SpeechAct requestAct)
{
    srg::dialogue::AnswerGraph* answerGraph = new srg::dialogue::AnswerGraph();
    conceptnet::Concept* root = this->cn->getConcept(answerGraph, requestAct.text); // << should be cup
    std::stringstream ss;
    ss << srg::world::RoomType::Floor;
    std::cout << "[InformHandler] kitchen/room Relatedness:" << this->cn->getRelatedness("kitchen", "room") << std::endl;
    ss.clear();

    ss << srg::world::RoomType::Office;
    std::cout << "[InformHandler] Cup/Office Relatedness:" << this->cn->getRelatedness(root->term, ss.str()) << std::endl;
    ss.clear();

    ss << srg::world::RoomType::Kitchen;
    std::cout << "[InformHandler] Cup/Kitchen Relatedness:" << this->cn->getRelatedness(root->term, ss.str()) << std::endl;
    ss.clear();

    ss << srg::world::RoomType::Bathroom;
    std::cout << "[InformHandler] Cup/Bathroom Relatedness:" << this->cn->getRelatedness(root->term, ss.str()) << std::endl;
    ss.clear();

    ss << srg::world::RoomType::ConferenceRoom;
    std::cout << "[InformHandler] Cup/ConferenceRoom Relatedness:" << this->cn->getRelatedness(root->term, ss.str()) << std::endl;
    ss.clear();

    ss << srg::world::RoomType::ReceptionRoom;
    std::cout << "[InformHandler] Cup/ReceptionRoom Relatedness:" << this->cn->getRelatedness(root->term, ss.str()) << std::endl;
    ss.clear();

    ss << srg::world::RoomType::ServerRoom;
    std::cout << "[InformHandler] Cup/ServerRoom Relatedness:" << this->cn->getRelatedness(root->term, ss.str()) << std::endl;
    ss.clear();

    ss << srg::world::RoomType::Storeroom;
    std::cout << "[InformHandler] Cup/Storeroom Relatedness:" << this->cn->getRelatedness(root->term, ss.str()) << std::endl;
    ss.clear();

    ss << srg::world::RoomType::UtilityRoom;
    std::cout << "[InformHandler] Cup/UtilityRoom Relatedness:" << this->cn->getRelatedness(root->term, ss.str()) << std::endl;
    ss.clear();

    ss << srg::world::RoomType::WorkshopRoom;
    std::cout << "[InformHandler] Cup/WorkshopRoom Relatedness:" << this->cn->getRelatedness(root->term, ss.str()) << std::endl;
    ss.clear();

    std::cout << answerGraph->toString() << std::endl;
    return nullptr;
}
} // namespace dialogue
} // namespace srg