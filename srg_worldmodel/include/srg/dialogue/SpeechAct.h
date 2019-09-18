#pragma once

#include <essentials/IdentifierConstPtr.h>

namespace srg
{

namespace dialogue
{
class AnswerGraph;
enum SpeechType
{
    inform,
    request,
    command
};

class SpeechAct
{
public:
    SpeechAct();
    essentials::IdentifierConstPtr senderID;
    essentials::IdentifierConstPtr actID;
    essentials::IdentifierConstPtr previousActID;

    SpeechType type;
    std::string text;
    srg::dialogue::AnswerGraph* answerGraph;
};
} // namespace wm
} // namespace srg
