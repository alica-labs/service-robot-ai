#pragma once

#include <srgsim/world/ObjectType.h>

#include <vector>

namespace srgsim {
    class Cell;
}

namespace srg
{
namespace wm
{
class Search
{
public:
    Search(srgsim::ObjectType objectType);

    // todo: somehow init it and pass the perceptions to it? or just make it update according to current position
    void update();
    srgsim::Cell* getNextCell();

private:
    srgsim::ObjectType objectType;
    std::vector<srgsim::Cell*> fringe;
};
} // namespace wm
} // namespace srg
