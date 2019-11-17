#pragma once

#include "srg/SRGWorldModel.h"
#include "Task.h"
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

    const supplementary::InfoBuffer<Task*>& getTaskActBuffer();
    std::shared_ptr<const supplementary::InformationElement<Task*>> getActiveTask() const;

private:
    srg::SRGWorldModel* wm;
    TaskFactory* taskFactory;
    essentials::SystemConfig* sc;
    alica::AlicaTime taskValidityDuration;
    supplementary::InfoBuffer<Task*>* taskActBuffer;
    std::shared_ptr<const supplementary::InformationElement<Task*>> activeTask;
};
} // namespace dialogue
} // namespace srg
