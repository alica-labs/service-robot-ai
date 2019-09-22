#include "srg/dialogue/CommandHandler.h"

#include <SystemConfig.h>

namespace srg
{
namespace dialogue
{
CommandHandler::CommandHandler(SRGWorldModel* wm)
        : wm(wm)
{
    auto sc = essentials::SystemConfig::getInstance();
    this->commandActValidityDuration = alica::AlicaTime::nanoseconds((*sc)["SRGWorldModel"]->get<int>("Data.CommandAct.ValidityDuration", NULL));
    this->commandActBuffer = new supplementary::InfoBuffer<control::SpeechAct>((*sc)["SRGWorldModel"]->get<int>("Data.CommandAct.BufferLength", NULL));
}

void CommandHandler::processCommandAct(std::shared_ptr<supplementary::InformationElement<control::SpeechAct>> commandAct)
{
    this->commandActBuffer->add(commandAct);
}

const supplementary::InfoBuffer<control::SpeechAct>& CommandHandler::getCommandActBuffer()
{
    return *this->commandActBuffer;
}
} // namespace dialogue
} // namespace srg