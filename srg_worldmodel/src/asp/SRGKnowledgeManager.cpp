#include "srg/asp/SRGKnowledgeManager.h"

#include <reasoner/asp/Solver.h>

namespace srg
{
namespace asp
{
SRGKnowledgeManager::SRGKnowledgeManager() {}

SRGKnowledgeManager::~SRGKnowledgeManager() {}

void SRGKnowledgeManager::setSolver(reasoner::asp::Solver * solver) {
    this->gen = new Generator(solver->WILDCARD_POINTER, solver->WILDCARD_STRING);
    ASPKnowledgeManager::setSolver(solver);
}

void SRGKnowledgeManager::ask(const std::string& question) {
    std::cout << "[SRGKnowledgeManager] Asking the following question '" << question << "'" << std::endl;
}

void SRGKnowledgeManager::addRoom(const srg::world::Room* room)
{
    std::vector<std::string> backgroundRules;
    backgroundRules.push_back(gen->is(room));
    this->addBackgroundRules(backgroundRules);
}

} // namespace asp
} // namespace srg