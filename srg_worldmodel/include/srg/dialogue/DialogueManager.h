#pragma once

#include <map>
#include <memory>
#include <vector>

namespace supplementary
{
template <typename>
class InformationElement;
}

namespace control
{
class SpeechAct;
}

namespace srg
{
namespace tasks
{
class TaskHandler;
}
class SRGWorldModel;
namespace dialogue
{
class AnswerGraph;
class InformHandler;
class TaskHandler;
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
    srg::tasks::TaskHandler* taskHandler;

private:
    srg::SRGWorldModel* wm;
    std::vector<std::shared_ptr<control::SpeechAct>> speechActs;
    void renderDot() const;
};
} // namespace dialogue
} // namespace srg
