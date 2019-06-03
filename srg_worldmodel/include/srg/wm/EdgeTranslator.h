#pragma once

#include "srg/container/Edge.h"

#include <string>
#include <vector>

namespace srg
{
namespace wm
{
class EdgeTranslator
{
public:
    EdgeTranslator() = delete;
    static std::string translate(std::vector<srg::container::Edge>& edges);

private:
    static std::string createBackgroundKnowledgeRule(std::string& relation, srg::container::Edge& edge);
    static std::string conceptToASPPredicate(std::string concept);
    static std::string createASPPredicates(std::vector<srg::container::Edge>& edges);
    /**
     * Containts the cn5_ prefix.
     */
    static const std::string PREFIX;
    /**
 * Containts the commonsense knowledge section.
 */
    static const std::string COMMONSENSE_KNOWLEDGE;
    /**
 * Containts the situational knowledge section.
 */
    static const std::string SITUATIONAL_KNOWLEDGE;
};

} // namespace wm
} // namespace srg
