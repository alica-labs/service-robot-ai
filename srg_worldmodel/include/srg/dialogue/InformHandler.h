#pragma once

#include <memory>
#include <string>

#include <srg/agent/containers/SpeechAct.h>

namespace srg
{
class SRGWorldModel;
namespace conceptnet
{
class ConceptNet;
}
namespace dialogue
{
class InformHandler
{
public:
    explicit InformHandler(SRGWorldModel* wm);

    std::shared_ptr<agent::SpeechAct> answerInform(const agent::SpeechAct informAct);

private:
    std::shared_ptr<agent::SpeechAct> getLocations(const agent::SpeechAct informAct,const std::string& locationType);
    std::shared_ptr<agent::SpeechAct> createAnswerSpeechAct(essentials::IdentifierConstPtr previousActID, srg::dialogue::AnswerGraph* answerGraph, agent::SpeechType type);
    std::shared_ptr<agent::SpeechAct> testGetLocationsViaRelatedness(const agent::SpeechAct informAct);
    std::shared_ptr<agent::SpeechAct> testInconsistencyStuff(const agent::SpeechAct informAct);
    srg::SRGWorldModel* wm;
    conceptnet::ConceptNet* cn;
};

} // namespace dialogue
} // namespace srg
