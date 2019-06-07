#pragma once

#include <essentials/IdentifierConstPtr.h>

namespace srg
{
namespace conceptnet
{
class AnswerGraph;
}

namespace dialogue
{
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
    conceptnet::AnswerGraph* answerGraph;
};
} // namespace wm
} // namespace srg
