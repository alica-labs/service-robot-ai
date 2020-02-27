#include "srg/asp/SRGKnowledgeManager.h"

#include <reasoner/asp/Solver.h>

namespace srg
{
namespace asp
{
SRGKnowledgeManager::SRGKnowledgeManager()
{
    this->perceptionExternals = std::make_shared<std::map<std::string, bool>>();
}

SRGKnowledgeManager::~SRGKnowledgeManager() {}

void SRGKnowledgeManager::setSolver(reasoner::asp::Solver* solver)
{
    this->gen = new Generator(solver->WILDCARD_POINTER, solver->WILDCARD_STRING);
    ASPKnowledgeManager::setSolver(solver);
}

std::vector<std::string> SRGKnowledgeManager::ask(const std::string& question)
{
    std::cout << "[SRGKnowledgeManager] Asking the following question '" << question << "'" << std::endl;
    return this->filterModel(question);
}

void SRGKnowledgeManager::handleObjects(std::vector<std::shared_ptr<srg::world::Object>> objects, bool seen)
{
    std::vector<std::string> perceptionRules;
    for (auto object : objects) {
        std::string perception = this->gen->is(object.get(), false);
        if (this->perceptionExternals->find(perception) == this->perceptionExternals->end()) {
            // object is perceived the first time, so add external to asp knowledge base
            perceptionRules.push_back("#external " + this->gen->is(object.get()));
            this->perceptionExternals->emplace(perception, seen);
        } else {
            // object is already known
            this->perceptionExternals->at(perception) = seen;
        }
    }

    if (perceptionRules.size() > 0) {
        this->addRulesPermanent("sensorInput", perceptionRules);
    }
    this->updateExternals(perceptionExternals);
}

void SRGKnowledgeManager::addRoom(const srg::world::Room* room)
{
    std::vector<std::string> backgroundRules;
    backgroundRules.push_back(this->gen->is(room));
    this->addRulesPermanent("base", backgroundRules);
}

} // namespace asp
} // namespace srg