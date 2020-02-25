#include "srg/dialogue/InformHandler.h"

#include "srg/asp/ASPTranslator.h"
#include "srg/conceptnet/Concept.h"
#include "srg/conceptnet/ConceptNet.h"
#include "srg/conceptnet/ConceptPath.h"
#include "srg/dialogue/AnswerGraph.h"

#include "srg/SRGWorldModel.h"

#include <engine/AlicaEngine.h>

namespace srg
{
namespace dialogue
{

InformHandler::InformHandler(SRGWorldModel* wm)
        : wm(wm)
{
    this->cn = this->wm->conceptNet;
}

std::shared_ptr<agent::SpeechAct> InformHandler::answerInform(const agent::SpeechAct informAct)
{
    return getLocations(informAct, "room");
}

std::shared_ptr<agent::SpeechAct> InformHandler::createAnswerSpeechAct(essentials::IdentifierConstPtr previousActID, srg::dialogue::AnswerGraph* answerGraph, agent::SpeechType type)
{
    std::shared_ptr<agent::SpeechAct> answerSpeechAct = std::make_shared<agent::SpeechAct>();
    answerSpeechAct->text = "";
    answerSpeechAct->answerGraph = answerGraph;
    answerSpeechAct->type = type;
    answerSpeechAct->previousActID = previousActID;
    answerSpeechAct->actID = this->wm->getEngine()->getIdManager()->generateID();
    answerSpeechAct->senderID = this->wm->getOwnId();
    return answerSpeechAct;
}

std::shared_ptr<agent::SpeechAct> InformHandler::getLocations(const agent::SpeechAct informAct, const std::string& locationType)
{
    srg::dialogue::AnswerGraph* answerGraph = new srg::dialogue::AnswerGraph();
    answerGraph->root = this->cn->getConcept(answerGraph, informAct.text); // << should be SearchedItem
    if (!answerGraph->root) {
        // did not find any concept for the given text
        std::cout << "[InformHandler] No root concept found for '" << informAct.text << "'" << std::endl;
        return nullptr; //createAnswerSpeechAct(informAct.actID, answerGraph, agent::SpeechType::request);
    }

    // init fringe
    std::vector<conceptnet::ConceptPath*> fringe;
    conceptnet::ConceptPath* path = new conceptnet::ConceptPath(answerGraph->root); // SearchedItem
    fringe.push_back(path);
    for (conceptnet::Edge* edge : this->cn->getEquivalentOutgoingEdges(answerGraph, answerGraph->root, -1)) {
        conceptnet::ConceptPath* path = new conceptnet::ConceptPath(answerGraph->root);
        path->addToPath(edge); // things similar to SearchedItem
        fringe.push_back(path);
    }

    while (!fringe.empty()) {
        conceptnet::ConceptPath* currentPath = fringe[0];
        fringe.erase(fringe.begin());
//        std::cout << "[InformHandler] CurrentPath: " << std::endl << currentPath->toString("\t") << std::endl;

        // GOAL CHECK
        std::vector<conceptnet::Edge*> locationTypeCheckEdges =
                this->cn->getCompleteEdges(answerGraph, conceptnet::Relation::IsA, currentPath->getEnd()->term, locationType, -1);
        if (!locationTypeCheckEdges.empty()) {
            answerGraph->answerPaths.push_back(currentPath);
            if (std::find(answerGraph->answerConcepts.begin(), answerGraph->answerConcepts.end(), currentPath->getEnd()) == answerGraph->answerConcepts.end()) {
                answerGraph->answerConcepts.push_back(currentPath->getEnd());
            }
            continue;
        }

        // LIMIT SEARCH DEPTH
        if (currentPath->getPath().size() == 3) {
            delete currentPath;
            continue;
        }

        // EXPAND SEARCH
        bool expanded = false;
        for (conceptnet::Edge* edge : this->cn->getOutgoingEdges(answerGraph, conceptnet::Relation::AtLocation, currentPath->getEnd()->term, -1)) {
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

    std::vector<conceptnet::Concept*> answers = answerGraph->getBestAnswers(1);
    std::cout << "[InformHandler] " << std::endl;
    for (conceptnet::Concept* concept : answers) {
        std::cout << concept->term << std::endl;
    }
    //    std::cout << answerGraph->toString() << std::endl;
    this->wm->aspTranslator->extractASPProgram(answerGraph, asp::ASPTranslator::InconsistencyRemoval::UseExternals);

    return nullptr;//createAnswerSpeechAct(informAct.actID, answerGraph, agent::SpeechType::request);
}

std::shared_ptr<agent::SpeechAct> InformHandler::testInconsistencyStuff(const agent::SpeechAct informAct)
{
    srg::dialogue::AnswerGraph* answerGraph = new srg::dialogue::AnswerGraph();
    answerGraph->root = this->cn->getConcept(answerGraph, informAct.text);

    answerGraph->root->addEdges(this->cn->getEdges(answerGraph, answerGraph->root->term, -1));

    this->cn->findInconsistencies(answerGraph);
    // answerGraph->renderDot(true);
    std::ofstream file;
    file.open("example.lp");
    file << this->wm->aspTranslator->extractASPProgram(answerGraph, asp::ASPTranslator::InconsistencyRemoval::UseExternals);
    file.close();
    std::cout << answerGraph->toString() << std::endl;

   return createAnswerSpeechAct(informAct.actID, answerGraph, agent::SpeechType::inform);
}

std::shared_ptr<agent::SpeechAct> InformHandler::testGetLocationsViaRelatedness(const agent::SpeechAct informAct)
{
    srg::dialogue::AnswerGraph* answerGraph = new srg::dialogue::AnswerGraph();
    conceptnet::Concept* root = this->cn->getConcept(answerGraph, informAct.text); // << should be cup
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
