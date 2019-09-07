#pragma once

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
class InformHandler;
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
    std::map <std::shared_ptr<supplementary::InformationElement<SpeechAct>>, AnswerGraph*> actMapping;

    int counter;
    void renderDot(AnswerGraph* anserGraph);
};
} // namespace dialogue
} // namespace srg