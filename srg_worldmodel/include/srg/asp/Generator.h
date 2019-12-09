#pragma once

#include <essentials/IdentifierConstPtr.h>

#include <string>
#include <map>

namespace srg
{
namespace world
{
class Room;
}
namespace asp
{
class Generator
{
public:
    Generator(const void* wildcard_pointer, std::string wildcard_string);

    // BINARY PREDICATES
    std::string is(const srg::world::Room* r, bool dotTerminated = true);

    // LITERALS
    std::string get(const srg::world::Room* room);

private:
    // maps from id to asp std::string
    std::map<essentials::IdentifierConstPtr, std::string> elements;

    const void* wildcard_pointer;
    std::string wildcard_string;
};
} // namespace asp
} // namespace srg
