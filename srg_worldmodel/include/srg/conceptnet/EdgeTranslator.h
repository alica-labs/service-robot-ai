#pragma once

#include "srg/conceptnet/Edge.h"

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
    static std::string translate(std::vector<srg::conceptnet::Edge>& edges);

private:
    static std::string createBackgroundKnowledgeRule(std::string& relation, srg::conceptnet::Edge& edge);
    static std::string conceptToASPPredicate(std::string concept);
    static std::string createASPPredicates(std::vector<srg::conceptnet::Edge>& edges);
    /**
     * Contains the cn5_ prefix.
     */
    static const std::string PREFIX;
    /**
     * Contains the commonsense knowledge section.
     */
    static const std::string COMMONSENSE_KNOWLEDGE;
    /**
     * Contains the situational knowledge section.
     */
    static const std::string SITUATIONAL_KNOWLEDGE;
};

} // namespace wm
} // namespace srg
