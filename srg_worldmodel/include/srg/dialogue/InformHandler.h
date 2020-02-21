#pragma once

#include <memory>
#include <string>

#include <srg/agent/containers/SpeechAct.h>

namespace srg
{
class SRGWorldModel;
namespace conceptnet
{
class ConceptNet;
}
namespace dialogue
{
class InformHandler
{
public:
    explicit InformHandler(SRGWorldModel* wm);

    std::shared_ptr<agent::SpeechAct> answerInform(const agent::SpeechAct informAct);

private:
    srg::SRGWorldModel* wm;
    conceptnet::ConceptNet* cn;
};

} // namespace dialogue
} // namespace srg
