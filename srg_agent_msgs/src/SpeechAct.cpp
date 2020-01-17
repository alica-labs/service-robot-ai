#include "srg/agent/containers/SpeechAct.h"

#include <iostream>

namespace srg
{
namespace agent
{
/**
 * For getting a string representation of a type of a speech act.
 * @param os Outputstream
 * @param type The type of a task.
 * @return Outputstream
 */
std::ostream& operator<<(std::ostream& os, const SpeechType& speechType)
{
    switch (speechType) {
    case SpeechType::command:
        os << "Command";
        break;
    case SpeechType::inform:
        os << "Inform";
        break;
    case SpeechType::request:
        os << "Request";
        break;
    default:
        os.setstate(std::ios_base::failbit);
    }
    return os;
}
} // namespace agent
} // namespace srg