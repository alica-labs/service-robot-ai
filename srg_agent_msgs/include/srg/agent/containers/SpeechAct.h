#pragma once

#include <essentials/IdentifierConstPtr.h>

#include <iosfwd>
#include <string>

namespace srg
{
namespace dialogue
{
class AnswerGraph;
}
} // namespace srg

namespace srg
{
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

    friend std::ostream& operator<<(std::ostream& os, const srg::agent::SpeechAct& act) {
        os << "[SpeechAct] Type: " << act.type << " Text: " << act.text;
        os <<  "Sender: " << act.senderID << " Receiver: " << act.receiverID;
        os << " ActID: " << act.actID << " Previous ActID: " << act.previousActID;
        return os;
    }
};
} // namespace human
} // namespace srg