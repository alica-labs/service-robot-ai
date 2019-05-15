#include "srg/wm/ASPKnowledgeBase.h"
#include <reasoner/asp/Term.h>
#include <reasoner/asp/Variable.h>
#include <reasoner/asp/AnnotatedValVec.h>
#include <reasoner/asp/Solver.h>
#include <reasoner/asp/ExtensionQuery.h>
#include <asp_solver_wrapper/ASPSolverWrapper.h>

#define ASPKB_DEBUG

namespace srg
{
namespace wm
{

ASPKnowledgeBase::ASPKnowledgeBase(KnowledgeManager* km)
        : km(km)
        , solver(nullptr)
{
}

ASPKnowledgeBase::~ASPKnowledgeBase() {}

/**
 *TODO would it be better to filter results with a facts query?
 * Solves the current ASP Program of the Knowledge Base according to specified queryRule
 */
std::vector<std::string> ASPKnowledgeBase::solve(std::string queryRule)
{
    auto vars = std::vector<reasoner::asp::Variable*>();
    vars.push_back(new reasoner::asp::Variable());
    auto terms = std::vector<reasoner::asp::Term*>();
    std::vector<reasoner::asp::AnnotatedValVec*> results;

    auto ret = std::vector<std::string>();
    auto t = new ::reasoner::asp::Term(1);
    t->setQueryRule(queryRule);
    t->setType(::reasoner::asp::QueryType::Filter);
    t->setProgramSection("wumpusBackgroundKnowledgeFile");
    std::lock_guard<std::mutex> lock(mtx);
    auto queryId = this->solver->getRegisteredQueriesCount();
    // TODO set ids properly
    //============
    t->setId(queryId);
    //============
    t->setQueryId(queryId);
    terms.push_back(t);
    this->solver->getSolution(vars, terms, results);
    if (results.size() > 0) {

#ifdef ASPKB_DEBUG
        std::cout << "ASPKB: Found Result!" << std::endl;
#endif

        for (auto res : results) {

            for (size_t i = 0; i < res->factQueryValues.size(); ++i) {
                for (size_t j = 0; j < res->factQueryValues.at(i).size(); ++j) {
                    auto elem = res->factQueryValues.at(i).at(j);
                    std::cout << "ASPKB: " << i << "," << j << ", " << elem << std::endl;
                    this->currentActionSequence.push_back(elem);
                }
            }
        }
    }

    //    for (auto m : this->solver->getCurrentModels())
    //    {
    //        for (auto n : m)
    //        {
    //            cout << "GET CURRENT MODELS: " << n << endl;
    //        }
    //    }

    for (auto var : vars) {
        delete var;
    }

    for (auto term : terms) {
        delete term;
    }

    // TODO
    return this->currentActionSequence;
}

/**
 * Combines information into a term, wraps it in a query and registers it in the solver
 * @return id of query
 */
int ASPKnowledgeBase::addInformation(std::vector<std::string>& information, int lifetime)
{
    std::lock_guard<std::mutex> lock(mtx);
    ::reasoner::asp::Term* term = new ::reasoner::asp::Term(lifetime);
    int queryId = this->solver->getRegisteredQueriesCount();
    //    term->setProgramSection("wumpusBackgroundKnowledgeFile");
    term->setQueryId(queryId);
    std::stringstream ss;
    for (auto inf : information) {
        term->addFact(inf);
    }

    std::shared_ptr<::reasoner::asp::Query> query = std::make_shared<::reasoner::asp::ExtensionQuery>(this->solver, term);
    bool success = this->solver->registerQuery(query);

#ifdef ASPKB_DEBUG
        std::cout << "ASPKB: Adding query " << queryId << " was " << (success ? "successful" : "not successful") << std::endl;
#endif

    if (success) {
        return queryId;
    }

    return -1;
}

/**
 * Removes a specified query from solver
 */
bool ASPKnowledgeBase::revoke(int queryId)
{
    std::lock_guard<std::mutex> lock(mtx);
    bool success = false;
    auto registered = this->solver->getRegisteredQueries();

    for (auto query : registered) {
        int registeredId = query->getTerm()->getQueryId();
        if (registeredId == queryId) {
            success = solver->unregisterQuery(query);
        }
    }

#ifdef ASPKB_DEBUG
        std::cout << "ASPKB: Removing query " << queryId << " was " << (success ? "successful" : "not successful") << std::endl;
#endif
    return success;
}

/**
 * Helper method. Solver can't be instantiated at construction time
 */
void ASPKnowledgeBase::initializeSolver(::reasoner::asp::Solver* solver)
{
    if (this->solver == nullptr) {
#ifdef ASPKB_DEBUG
        std::cout << "ASPKB: Initialize Solver." << std::endl;
#endif
        this->solver = solver;
    }
}

} /* namespace wm */
} /* namespace srg */
