#pragma once

#include "srg/asp/Generator.h"

#include <srg/world/Object.h>
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

    std::vector<std::string> ask(const std::string& question);
    void handleObjects(std::vector<std::shared_ptr<srg::world::Object>> objects, bool seen);
    void addRoom(const srg::world::Room* room);
private:
    Generator* gen;

    /**
     * This map manages the perceptions, which are represented
     * as externals in ASP:
     * Example: key = is(object3,cup), value = true
     */
    std::shared_ptr<std::map<std::string, bool>> perceptionExternals;
};
} // namespace asp
} // namespace srg
