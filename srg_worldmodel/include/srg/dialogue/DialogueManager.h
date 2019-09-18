#pragma once

#include <memory>
#include <map>
#include <vector>

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
class InformHandler;
class CommandHandler;
class BasicHumanNeeds;

class DialogueManager
{
public:
    explicit DialogueManager(SRGWorldModel* wm);
    ~DialogueManager();
    void processSpeechAct(std::shared_ptr<supplementary::InformationElement<SpeechAct>> speechAct);

private:
    srg::SRGWorldModel* wm;
    BasicHumanNeeds* basicHumanNeeds;
    InformHandler* informHandler;
    CommandHandler* commandHandler;
    std::vector<std::shared_ptr<SpeechAct>> speechActs;
    void renderDot() const;
};
} // namespace dialogue
} // namespace srg
