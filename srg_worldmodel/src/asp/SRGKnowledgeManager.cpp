#include "srg/asp/SRGKnowledgeManager.h"

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

int SRGKnowledgeManager::addRoom(const srg::world::Room* room)
{
    std::vector<std::string> infoStrings;
    infoStrings.push_back(gen->get(room));
    return this->addInformation(infoStrings);
}

} // namespace asp
} // namespace srg