#pragma once

#include <map>
#include <memory>
#include <vector>

namespace srg
{
class SRGWorldModel;
namespace wm
{
class ASPKnowledgeBase;
class KnowledgeManager
{
public:
    KnowledgeManager(SRGWorldModel* wm);
    virtual ~KnowledgeManager();
    void initializeSolver();

    void updateKnowledgeBase(std::string info);

private:
    ASPKnowledgeBase* kb;
    SRGWorldModel* wm;
    int64_t queryCount;

};

} /* namespace wm */
} /* namespace srg */
