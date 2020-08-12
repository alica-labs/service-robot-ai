#pragma once

#include "srg/SRGWorldModel.h"
#include "srg/tasks/Task.h"
#include "srg/tasks/TaskSequence.h"

#include <srg/agent/containers/SpeechAct.h>
#include <nonstd/optional.hpp>

namespace essentials
{
class SystemConfig;
}

namespace srg
{
namespace tasks
{
class TaskFactory;
class CommandHandler
{
public:
    CommandHandler(SRGWorldModel* wm);
    virtual ~CommandHandler();
    void tick();
    std::shared_ptr<agent::SpeechAct> handle(const std::shared_ptr<const supplementary::InformationElement<agent::SpeechAct>> commandAct);

    const supplementary::InfoBuffer<agent::SpeechAct>& getTaskActBuffer();
    std::shared_ptr<TaskSequence> getActiveTaskSequence();

private:
    void updateCurrentTaskSequence();
    void removeInvalidKnowledge();
    void propagateKnowledge();
    void setNextTaskSequence();
    void logTaskSequence(std::shared_ptr<TaskSequence> taskSequence);

    srg::SRGWorldModel* wm;
    TaskFactory* taskFactory;
    essentials::SystemConfig* sc;
    alica::AlicaTime taskValidityDuration;
    supplementary::InfoBuffer<agent::SpeechAct>* taskActBuffer;
    std::shared_ptr<TaskSequence> currentTaskSequence;

    std::string logPath;
    std::ofstream fileWriter;
};
} // namespace tasks
} // namespace srg
