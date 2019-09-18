#include "srg/wm/RawSensorData.h"

#include "srg/SRGWorldModel.h"

#include "srg/dialogue/SpeechAct.h"

#include <SystemConfig.h>
#include <supplementary/InfoBuffer.h>

#include <memory>
#include <srgsim/containers/SimPerceptions.h>

namespace srg
{
namespace wm
{
RawSensorData::RawSensorData(srg::SRGWorldModel* wm)
{
    this->wm = wm;
    auto sc = essentials::SystemConfig::getInstance();
    this->telegramMessageValidityDuration = alica::AlicaTime::nanoseconds((*sc)["SRGWorldModel"]->get<int>("Data.Telegram.ValidityDuration", NULL));
    this->telegramMessageBuffer = new supplementary::InfoBuffer<Message>((*sc)["SRGWorldModel"]->get<int>("Data.Telegram.BufferLength", NULL));

    this->speechActValidityDuration = alica::AlicaTime::nanoseconds((*sc)["SRGWorldModel"]->get<int>("Data.SpeechAct.ValidityDuration", NULL));
    this->speechActBuffer = new supplementary::InfoBuffer<srg::dialogue::SpeechAct>((*sc)["SRGWorldModel"]->get<int>("Data.SpeechAct.BufferLength", NULL));

    this->agentCmdValidityDuration = alica::AlicaTime::nanoseconds((*sc)["SRGWorldModel"]->get<int>("Data.AgentCmd.ValidityDuration", NULL));
    this->agentCmdBuffer = new supplementary::InfoBuffer<control::AgentCommand>((*sc)["SRGWorldModel"]->get<int>("Data.AgentCmd.BufferLength", NULL));

    this->perceptionsValidityDuration = alica::AlicaTime::nanoseconds((*sc)["SRGWorldModel"]->get<int>("Data.Perception.ValidityDuration", NULL));
    this->perceptionsBuffer = new supplementary::InfoBuffer<srgsim::SimPerceptions>((*sc)["SRGWorldModel"]->get<int>("Data.Perception.BufferLength", NULL));
}

RawSensorData::~RawSensorData() {}

const supplementary::InfoBuffer<Message>& RawSensorData::getTelegramMessageBuffer()
{
    return *this->telegramMessageBuffer;
}

const supplementary::InfoBuffer<control::AgentCommand>& RawSensorData::getAgentCmdBuffer()
{
    return *this->agentCmdBuffer;
}

const supplementary::InfoBuffer<srg::dialogue::SpeechAct>& RawSensorData::getSpeechActBuffer()
{
    return *this->speechActBuffer;
}

const supplementary::InfoBuffer<srgsim::SimPerceptions>& RawSensorData::getPerceptionsBuffer()
{
    return *this->perceptionsBuffer;
}

void RawSensorData::processTelegramMessage(Message message)
{
    std::cout << "RawSensorData: processTelegramMessage called" << std::endl;
    auto messageInfo = std::make_shared<supplementary::InformationElement<Message>>(message, wm->getTime(), telegramMessageValidityDuration, 1.0);
    telegramMessageBuffer->add(messageInfo);
}

void RawSensorData::processSpeechAct(srg::dialogue::SpeechAct act)
{
    auto speechActInfo = std::make_shared<supplementary::InformationElement<srg::dialogue::SpeechAct>>(act, wm->getTime(), speechActValidityDuration, 1.0);
    speechActBuffer->add(speechActInfo);
    this->wm->dialogueManager.processSpeechAct(speechActInfo);
}

void RawSensorData::processAgentCmd(control::AgentCommand agentCmd)
{
    auto agentCmdInfo = std::make_shared<supplementary::InformationElement<control::AgentCommand>>(agentCmd, wm->getTime(), agentCmdValidityDuration, 1.0);
    agentCmdBuffer->add(agentCmdInfo);
}

void RawSensorData::processSimPerceptions(srgsim::SimPerceptions simPerceptions)
{
    std::cout << "RawSensorData: processSimPerceptions called" << std::endl;
    auto perceptionsInfo = std::make_shared<supplementary::InformationElement<srgsim::SimPerceptions>>(simPerceptions, wm->getTime(), perceptionsValidityDuration, 1.0);
    perceptionsBuffer->add(perceptionsInfo);
}

} // namespace wm
} // namespace srg