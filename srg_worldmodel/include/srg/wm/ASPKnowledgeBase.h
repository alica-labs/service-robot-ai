#pragma once
#include <reasoner/asp/Solver.h>
#include <asp_solver_wrapper/ASPSolverWrapper.h>
#include <mutex>
#include <vector>

namespace srg
{
class WumpusWorldModel;
namespace wm
{
class KnowledgeManager;
class ASPKnowledgeBase
{
public:
    ASPKnowledgeBase(KnowledgeManager* wm);
    virtual ~ASPKnowledgeBase();
    void initializeSolver(::reasoner::asp::Solver* solver);
    std::vector<std::string> solve(std::string queryRule);
    int addInformation(std::vector<std::string>& information, int lifetime = -1);
    bool revoke(int queryId);

private:
    KnowledgeManager* km;
    ::reasoner::asp::Solver* solver;
    std::vector<std::string> currentActionSequence;
    std::mutex mtx;
};

} /* namespace wm */
} /* namespace srg */
