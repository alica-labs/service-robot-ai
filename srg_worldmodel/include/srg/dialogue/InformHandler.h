#pragma once

#include <string>
#include <memory>

namespace control {
    class SpeechAct;
}
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

    std::shared_ptr<control::SpeechAct> answerInform(const control::SpeechAct informAct);

private:
    srg::SRGWorldModel* wm;
    conceptnet::ConceptNet* cn;
};

} // namespace dialogue
} // namespace srg
