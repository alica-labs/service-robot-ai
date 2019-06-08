#pragma once

#include <essentials/IdentifierConstPtr.h>
#include <memory>
#include <map>

namespace supplementary {
    template<typename>
    class InformationElement;
}

namespace srg
{
class SRGWorldModel;
namespace dialogue
{
class AnswerGraph;
class SpeechAct;
class BasicHumanNeeds;

class DialogueManager
{
public:
    DialogueManager(SRGWorldModel* wm);
    ~DialogueManager();
    void processSpeechAct(std::shared_ptr<supplementary::InformationElement<SpeechAct>> speechAct);
private:
    srg::SRGWorldModel* wm;
    BasicHumanNeeds* basicHumanNeeds;
    std::map <essentials::IdentifierConstPtr, AnswerGraph*> actMapping;
};
} // namespace dialogue
} // namespace srg