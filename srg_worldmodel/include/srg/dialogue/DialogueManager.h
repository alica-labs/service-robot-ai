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
class SpeechAct;
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
    void processSpeechAct(std::shared_ptr<supplementary::InformationElement<SpeechAct>> speechAct);

    srg::SRGWorldModel* wm;
    BasicHumanNeeds* basicHumanNeeds;
    InformHandler* informHandler;
    CommandHandler* commandHandler;
private:
    std::vector<std::shared_ptr<SpeechAct>> speechActs;
    void renderDot() const;
};
} // namespace dialogue
} // namespace srg
