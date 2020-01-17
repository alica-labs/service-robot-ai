#include "srg/asp/Generator.h"

#include <srg/world/Room.h>
#include <sstream>

namespace srg
{
namespace asp
{
Generator::Generator(const void* wildcard_pointer, std::string wildcard_string)
        : wildcard_pointer(wildcard_pointer)
        , wildcard_string(std::move(wildcard_string))
{
}

// UNARY Predicates
std::string Generator::is(const srg::world::Room* room, bool dotTerminated)
{
    std::stringstream roomType;
    roomType << room->getType();
    return "is(" + get(room) + ", " + roomType.str() + (dotTerminated ? ")." : ")");
}

// LITERALS
std::string Generator::get(const srg::world::Room* room)
{
    if (room == this->wildcard_pointer)
        return this->wildcard_string;

    auto&& iter = this->elements.find(room->getID());
    if (iter == this->elements.end()) {
        return this->elements[room->getID()] = ("room" + room->getID().toString());
    }
    return iter->second;
}
} // namespace asp
} // namespace srg
