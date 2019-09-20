#pragma once

#include <string>
#include <memory>

namespace srg
{
class SpeechAct;
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

    std::shared_ptr<srg::SpeechAct> answerInform(const srg::SpeechAct informAct);

private:
    srg::SRGWorldModel* wm;
    conceptnet::ConceptNet* cn;
};

} // namespace dialogue
} // namespace srg
