#include "srg/asp/Generator.h"

#include <srg/world/Object.h>
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

// BINARY Predicates
/**
 * Returns a predicate of the form is(room123, kitchen).
 * @param room The queried room.
 * @param dotTerminated True, for facts. False, otherwise.
 * @return String representing the queried "is" predicate.
 */
std::string Generator::is(const srg::world::Object* object, bool dotTerminated)
{
    std::string objectTypeString;
    switch (object->getType()) {
        case srg::world::ObjectType::CupRed:
        case srg::world::ObjectType::CupYellow:
        case srg::world::ObjectType::CupBlue:
            objectTypeString = "cup";
            break;
        default:
            std::stringstream otStream;
            otStream << object->getType();
            objectTypeString = otStream.str();
            objectTypeString[0] = std::tolower(objectTypeString[0]);
    }

    return "is(" + get(object) + ", " + objectTypeString + (dotTerminated ? ")." : ")");
}

/**
 * Returns a predicate of the form is(room123, kitchen).
 * @param room The queried room.
 * @param dotTerminated True, for facts. False, otherwise.
 * @return String representing the queried "is" predicate.
 */
std::string Generator::is(const srg::world::Room* room, bool dotTerminated)
{
    std::stringstream roomType;
    roomType << room->getType();
    return "is(" + get(room) + ", " + roomType.str() + (dotTerminated ? ")." : ")");
}

// LITERALS
std::string Generator::get(const srg::world::Object* object)
{
    if (object == this->wildcard_pointer)
        return this->wildcard_string;

    auto&& iter = this->elements.find(object->getID());
    if (iter == this->elements.end()) {
        return this->elements[object->getID()] = ("object" + object->getID().toString());
    }
    return iter->second;
}

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
