#pragma once

#include "srg/asp/Generator.h"

#include <knowledge_manager/ASPKnowledgeManager.h>

namespace knowledge_manager
{
class ASPKnowledgeManager;
}

namespace srg
{
namespace world
{
class Room;
}
namespace asp
{

class SRGKnowledgeManager : public knowledge_manager::ASPKnowledgeManager
{
public:
    SRGKnowledgeManager();
    virtual ~SRGKnowledgeManager();
    void setSolver(reasoner::asp::Solver *solver) override;

    void ask(const std::string& question);
    void addRoom(const srg::world::Room* room);
private:
    Generator* gen;
};
} // namespace asp
} // namespace srg
