#pragma once

#include <string>

namespace srg
{
class SRGWorldModel;
namespace conceptnet
{
class ConceptNet;
}
namespace dialogue
{
class AnswerGraph;
class InformHandler
{
public:
    explicit InformHandler(SRGWorldModel* wm);

    AnswerGraph* answerInform(std::string need);

private:
    srg::SRGWorldModel* wm;
    conceptnet::ConceptNet* cn;
};

} // namespace dialogue
} // namespace srg
