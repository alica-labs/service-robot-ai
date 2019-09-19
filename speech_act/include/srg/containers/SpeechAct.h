#pragma once

#include <essentials/IdentifierConstPtr.h>

namespace srg
{

namespace dialogue
{
class AnswerGraph;
}
enum SpeechType : uint16_t
{
    inform,
    request,
    command
};

class SpeechAct
{
public:
    essentials::IdentifierConstPtr senderID;
    essentials::IdentifierConstPtr receiverID;
    essentials::IdentifierConstPtr actID;
    essentials::IdentifierConstPtr previousActID;

    SpeechType type;
    std::string text;
    dialogue::AnswerGraph* answerGraph;
};
} // namespace srg
