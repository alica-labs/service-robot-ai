#include "srg/wm/RawSensorData.h"

#include "srg/SRGWorldModel.h"

#include <srg/agent/containers/SpeechAct.h>

#include <SystemConfig.h>
#include <supplementary/InfoBuffer.h>

#include <memory>
#include <srg/sim/containers/Perceptions.h>
#include <essentials/WildcardID.h>

namespace srg
{
namespace wm
{
RawSensorData::RawSensorData(srg::SRGWorldModel* wm)
{
    this->wm = wm;
    auto sc = essentials::SystemConfig::getInstance();
    this->speechActValidityDuration = alica::AlicaTime::nanoseconds((*sc)["SRGWorldModel"]->get<int>("Data.SpeechAct.ValidityDuration", NULL));
    this->speechActBuffer = new supplementary::InfoBuffer<agent::SpeechAct>((*sc)["SRGWorldModel"]->get<int>("Data.SpeechAct.BufferLength", NULL));

    this->agentCmdValidityDuration = alica::AlicaTime::nanoseconds((*sc)["SRGWorldModel"]->get<int>("Data.AgentCmd.ValidityDuration", NULL));
    this->agentCmdBuffer = new supplementary::InfoBuffer<agent::AgentCommand>((*sc)["SRGWorldModel"]->get<int>("Data.AgentCmd.BufferLength", NULL));

    this->perceptionsValidityDuration = alica::AlicaTime::nanoseconds((*sc)["SRGWorldModel"]->get<int>("Data.Perception.ValidityDuration", NULL));
    this->perceptionsBuffer = new supplementary::InfoBuffer<srg::sim::containers::Perceptions>((*sc)["SRGWorldModel"]->get<int>("Data.Perception.BufferLength", NULL));
}

RawSensorData::~RawSensorData() {}

const supplementary::InfoBuffer<agent::AgentCommand>& RawSensorData::getAgentCmdBuffer()
{
    return *this->agentCmdBuffer;
}

const supplementary::InfoBuffer<agent::SpeechAct>& RawSensorData::getSpeechActBuffer()
{
    return *this->speechActBuffer;
}

const supplementary::InfoBuffer<srg::sim::containers::Perceptions>& RawSensorData::getPerceptionsBuffer()
{
    return *this->perceptionsBuffer;
}

void RawSensorData::processSpeechAct(agent::SpeechAct act)
{
    auto speechActInfo = std::make_shared<supplementary::InformationElement<agent::SpeechAct>>(act, wm->getTime(), speechActValidityDuration, 1.0);
    speechActBuffer->add(speechActInfo);

    // further processing
    this->wm->dialogueManager.processSpeechAct(speechActInfo);
}

void RawSensorData::processAgentCmd(agent::AgentCommand agentCmd)
{
    if (agentCmd.receiverID != this->wm->getOwnId() && agentCmd.receiverID->getType() != essentials::Identifier::WILDCARD_TYPE) {
        return;
    }
    auto agentCmdInfo = std::make_shared<supplementary::InformationElement<agent::AgentCommand>>(agentCmd, wm->getTime(), agentCmdValidityDuration, 1.0);
    agentCmdBuffer->add(agentCmdInfo);
}

void RawSensorData::processSimPerceptions(srg::sim::containers::Perceptions simPerceptions)
{
    auto perceptionsInfo = std::make_shared<supplementary::InformationElement<srg::sim::containers::Perceptions>>(simPerceptions, wm->getTime(), perceptionsValidityDuration, 1.0);
    perceptionsBuffer->add(perceptionsInfo);

    // further processing
    this->wm->sRGSimData.processPerception(perceptionsInfo->getInformation());
}

} // namespace wm
} // namespace srg
