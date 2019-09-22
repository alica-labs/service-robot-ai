#pragma once

#include <memory>
#include <map>
#include <vector>

namespace supplementary {
    template<typename>
    class InformationElement;
}

namespace control {
    class SpeechAct;
}

namespace srg
{
class SRGWorldModel;
namespace dialogue
{
class AnswerGraph;
class InformHandler;
class CommandHandler;
class BasicHumanNeeds;

#define inconsistency_eval
class DialogueManager
{
public:
    explicit DialogueManager(SRGWorldModel* wm);
    ~DialogueManager();
    void processSpeechAct(std::shared_ptr<supplementary::InformationElement<control::SpeechAct>> speechAct);

    BasicHumanNeeds* basicHumanNeeds;
    InformHandler* informHandler;
    CommandHandler* commandHandler;

private:
    srg::SRGWorldModel* wm;
    std::vector<std::shared_ptr<control::SpeechAct>> speechActs;
    void renderDot() const;
};
} // namespace dialogue
} // namespace srg
