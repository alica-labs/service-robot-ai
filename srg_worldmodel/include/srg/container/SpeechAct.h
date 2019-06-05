#pragma once

#include <essentials/IdentifierConstPtr.h>

namespace srg
{
namespace container
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
    SpeechType type;
    essentials::IdentifierConstPtr senderID;
    std::string text;
};
} // namespace container
} // namespace srg
