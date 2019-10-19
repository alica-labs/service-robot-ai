#include "srg/dialogue/CommandHandler.h"

#include <SystemConfig.h>

namespace srg
{
namespace dialogue
{
CommandHandler::CommandHandler(SRGWorldModel* wm)
        : wm(wm)
        , activeCommand(nullptr)
{
    auto sc = essentials::SystemConfig::getInstance();
    this->commandActValidityDuration = alica::AlicaTime::nanoseconds((*sc)["SRGWorldModel"]->get<int>("Data.CommandAct.ValidityDuration", NULL));
    this->commandActBuffer = new supplementary::InfoBuffer<control::SpeechAct>((*sc)["SRGWorldModel"]->get<int>("Data.CommandAct.BufferLength", NULL));
}

void CommandHandler::processCommandAct(std::shared_ptr<supplementary::InformationElement<control::SpeechAct>> commandAct)
{
    this->commandActBuffer->add(commandAct);
    if (!activeCommand) {
        this->activeCommand = commandAct;
    }
}

const supplementary::InfoBuffer<control::SpeechAct>& CommandHandler::getCommandActBuffer()
{
    return *this->commandActBuffer;
}

const nonstd::optional<control::SpeechAct> CommandHandler::getActiveCommand() const
{
    if (this->activeCommand == nullptr) {
        return nonstd::nullopt;
    }

    return activeCommand->getInformation();
}

void CommandHandler::commandSuccessful(essentials::IdentifierConstPtr actID)
{
    if (this->activeCommand && this->activeCommand->getInformation().actID == actID) {
        auto lastCommand = this->getCommandActBuffer().getLastValid();
        if (lastCommand->getInformation().actID == actID) {
            this->activeCommand = nullptr;
        } else {
            this->activeCommand = lastCommand;
        }
    }
}
} // namespace dialogue
} // namespace srg