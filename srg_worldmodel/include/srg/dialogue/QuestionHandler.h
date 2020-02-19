#pragma once

#include <memory>
#include <string>

#include <srg/agent/containers/SpeechAct.h>

namespace srg
{
class SRGWorldModel;
namespace dialogue
{
class BasicHumanNeeds;
class QuestionHandler
{
public:
    explicit QuestionHandler(SRGWorldModel* wm);

    std::shared_ptr<agent::SpeechAct> answerQuestion(const agent::SpeechAct requestAct);

private:
    srg::SRGWorldModel* wm;
    BasicHumanNeeds* basicHumanNeeds;
};
} // namespace dialogue
} // namespace srg
