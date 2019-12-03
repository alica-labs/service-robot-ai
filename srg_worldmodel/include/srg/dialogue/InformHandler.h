#pragma once

#include <string>
#include <memory>
#include <srg/agent/containers/SpeechAct.h>

namespace srg
{
class SRGWorldModel;
namespace conceptnet
{
class ConceptNet;
}
namespace agent {
    class SpeechAct;
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
