#pragma once

#include "srg/SRGWorldModel.h"
#include "srg/dialogue/Task.h"
#include <control/containers/SpeechAct.h>
#include <nonstd/optional.hpp>

namespace essentials
{
class SystemConfig;
}

namespace srg
{
namespace dialogue
{
class ManipulationTask;
class MoveTask;
class TaskHandler
{
public:
    TaskHandler(SRGWorldModel* wm);

    void tick();
    void processTaskAct(std::shared_ptr<supplementary::InformationElement<control::SpeechAct>> commandAct);

    const supplementary::InfoBuffer<Task*>& getTaskActBuffer();
    std::shared_ptr<const supplementary::InformationElement<Task*>> getActiveTask() const;

private:
    Task* createTask(std::shared_ptr<supplementary::InformationElement<control::SpeechAct>> taskAct);
    MoveTask* createMoveTask(std::string taskText);
    ManipulationTask* createManipulationTask(std::string taskText);
    bool isValid(srgsim::Coordinate coord, srgsim::TaskType type);

    srg::SRGWorldModel* wm;
    essentials::SystemConfig* sc;
    alica::AlicaTime taskValidityDuration;
    supplementary::InfoBuffer<Task*>* taskActBuffer;
    std::shared_ptr<const supplementary::InformationElement<Task*>> activeTask;
};
} // namespace dialogue
} // namespace srg
