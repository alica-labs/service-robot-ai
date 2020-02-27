#pragma once

#include <essentials/IdentifierConstPtr.h>
#include <srg/world/ObjectType.h>
#include <srg/world/RoomType.h>
#include <srg/sim/containers/Perceptions.h>

#include <iosfwd>
#include <string>

namespace srg
{
namespace dialogue
{
class AnswerGraph;
}
namespace agent
{
enum SpeechType : uint16_t
{
    inform,
    request,
    command
};
std::ostream& operator<<(std::ostream& os, const SpeechType& speechType);

class SpeechAct
{
public:
    essentials::IdentifierConstPtr senderID;
    essentials::IdentifierConstPtr receiverID;
    essentials::IdentifierConstPtr actID;
    essentials::IdentifierConstPtr previousActID;

    SpeechType type;
    std::string text;
    srg::dialogue::AnswerGraph* answerGraph;

    // Future Work: make these to fields more general
    srg::world::ObjectType objectRequestType;
    srg::sim::containers::Perceptions perceptions;
    std::vector<world::RoomType> probableRoomTypes;

    friend std::ostream& operator<<(std::ostream& os, const srg::agent::SpeechAct& act)
    {
        os << "[SpeechAct] Type: " << act.type << " Text: " << act.text;
        os << " Sender: " << act.senderID << " Receiver: " << act.receiverID;
        os << " ActID: " << act.actID << " Previous ActID: " << act.previousActID;
        return os;
    }
};
} // namespace agent
} // namespace srg