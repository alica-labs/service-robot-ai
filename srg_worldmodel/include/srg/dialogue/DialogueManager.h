#pragma once

#include <srg/agent/containers/SpeechAct.h>

#include <map>
#include <memory>
#include <vector>
#include <mutex>

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
class CommandHandler;
}
class SRGWorldModel;
namespace dialogue
{
class AnswerGraph;
class InformHandler;
class TaskHandler;
class RequestHandler;

//#define inconsistency_eval
class DialogueManager
{
public:
    explicit DialogueManager(SRGWorldModel* wm);
    ~DialogueManager();
    void processSpeechAct(const std::shared_ptr<const supplementary::InformationElement<agent::SpeechAct>> speechActInfo);
    void tick();

    RequestHandler* requestHandler;
    InformHandler* informHandler;
    srg::tasks::CommandHandler* commandHandler;

private:
    srg::SRGWorldModel* wm;
    std::unordered_map<essentials::IdentifierConstPtr, std::shared_ptr<agent::SpeechAct>> dialogueMap;
    std::vector<std::shared_ptr<agent::SpeechAct>> pendingSpeechActs;
    void renderDot() const;
    std::recursive_mutex _speechActMtx;
};
} // namespace dialogue
} // namespace srg
