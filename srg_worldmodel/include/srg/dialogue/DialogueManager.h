#pragma once

#include <memory>

namespace supplementary {
    template<typename>
    class InformationElement;
}

namespace srg
{
class SRGWorldModel;
namespace dialogue
{

class SpeechAct;
class BasicHumanNeeds;

class DialogueManager
{
public:
    DialogueManager(SRGWorldModel* wm);
    void processSpeechAct(std::shared_ptr<supplementary::InformationElement<SpeechAct>> speechAct);
private:
    srg::SRGWorldModel* wm;
    BasicHumanNeeds* basicHumanNeeds;
};
} // namespace dialogue
} // namespace srg