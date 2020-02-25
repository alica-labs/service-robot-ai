#pragma once

#include <srg/SRGWorldModel.h>
#include <srg/world/Cell.h>
#include <srg/world/Direction.h>

namespace srg
{
namespace agent
{
class Path
{
public:
    Path(srg::world::Coordinate start, srg::world::Coordinate goal, srg::SRGWorldModel* wm);
    Path(const Path& path);
    ~Path();
    srg::world::Direction getDirection();

    bool isGoalPath();
    int getTotalCosts();
    int getHeuristicCosts();
    std::vector<Path*> expand(std::vector<std::shared_ptr<const world::Cell>>& visited);
    std::shared_ptr<const world::Cell> getCell();
    friend std::ostream& operator<<(std::ostream& os, const srg::agent::Path& path)
    {
        os << "Path: ";
        const Path* currentPath = &path;
        while(currentPath->lastStep) {
            os << "(" << currentPath->cell->coordinate.x << ", " << currentPath->cell->coordinate.y << "), ";
            currentPath = currentPath->lastStep;
        }
        os << "(" << currentPath->cell->coordinate.x << ", " << currentPath->cell->coordinate.y << "), ";
        return os;
    }

private:
    Path* addStep(std::shared_ptr<const world::Cell> cell);
    bool checkValidity(std::vector<std::shared_ptr<const world::Cell>>& visited, std::shared_ptr<world::Cell> cell);

    int costs;
    std::shared_ptr<const world::Cell> cell;
    srg::world::Coordinate start;
    srg::world::Coordinate goal;
    Path* lastStep;
};
} // namespace agent
} // namespace srg