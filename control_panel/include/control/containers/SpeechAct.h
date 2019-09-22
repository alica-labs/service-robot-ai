#pragma once

#include <essentials/IdentifierConstPtr.h>

namespace srg {
    namespace dialogue
    {
        class AnswerGraph;
    }
}

namespace control
{
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
    srg::dialogue::AnswerGraph* answerGraph;
};
} // namespace srg
