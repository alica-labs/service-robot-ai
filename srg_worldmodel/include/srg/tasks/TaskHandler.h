#pragma once

#include "srg/SRGWorldModel.h"
#include "srg/tasks/Task.h"
#include "srg/tasks/TaskSequence.h"

#include <control/containers/SpeechAct.h>
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
class TaskHandler
{
public:
    TaskHandler(SRGWorldModel* wm);
    virtual ~TaskHandler();
    void tick();
    void processTaskAct(std::shared_ptr<supplementary::InformationElement<control::SpeechAct>> commandAct);

    const supplementary::InfoBuffer<control::SpeechAct>& getTaskActBuffer();
    std::shared_ptr<TaskSequence> getActiveTaskSequence();

private:
    void updateCurrentTaskSequence();
    void setNextTaskSequence();
    srg::SRGWorldModel* wm;
    TaskFactory* taskFactory;
    essentials::SystemConfig* sc;
    alica::AlicaTime taskValidityDuration;
    supplementary::InfoBuffer<control::SpeechAct>* taskActBuffer;
    std::shared_ptr<TaskSequence> currentTaskSequence;
};
} // namespace tasks
} // namespace srg
