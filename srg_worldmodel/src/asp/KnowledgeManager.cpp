#include "srg/wm/KnowledgeManager.h"

#include "srg/SRGWorldModel.h"
#include "srg/wm/ASPKnowledgeBase.h"

#include <reasoner/asp/Solver.h>
#include <asp_solver_wrapper/ASPSolverWrapper.h>
#include <engine/AlicaEngine.h>

#define KM_DEBUG

namespace srg
{
namespace wm
{

KnowledgeManager::KnowledgeManager(SRGWorldModel* wm)
        : wm(wm)
{
    this->kb = new ASPKnowledgeBase(this);
    this->queryCount = 0;
}

KnowledgeManager::~KnowledgeManager()
{
    delete this->kb;
}

void KnowledgeManager::initializeSolver()
{
    auto solver = this->wm->getEngine()->getSolver<alica::reasoner::ASPSolverWrapper>()->getSolver();
    this->kb->initializeSolver(static_cast<::reasoner::asp::Solver*>(solver));
}

/**
 * this is only intended for setting constants like field size
 */
void KnowledgeManager::updateKnowledgeBase(std::string info)
{
    auto information = std::vector<std::string>();
    information.push_back(info);
    this->kb->addInformation(information);
}

} /* namespace wm */
} /* namespace srg */
