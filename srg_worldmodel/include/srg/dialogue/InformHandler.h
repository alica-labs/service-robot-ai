#pragma once

#include <string>
#include <memory>

namespace srg
{
class SRGWorldModel;
namespace conceptnet
{
class ConceptNet;
}
namespace dialogue
{
class SpeechAct;
class InformHandler
{
public:
    explicit InformHandler(SRGWorldModel* wm);

    std::shared_ptr<SpeechAct> answerInform(const SpeechAct informAct);

private:
    srg::SRGWorldModel* wm;
    conceptnet::ConceptNet* cn;
};

} // namespace dialogue
} // namespace srg
