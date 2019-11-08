#pragma once

#include <srg/SRGWorldModel.h>
#include <srgsim/world/Cell.h>
#include <srgsim/world/Direction.h>

namespace srg
{
namespace robot
{
class Path
{
public:
    Path(srgsim::Coordinate start, srgsim::Coordinate goal, srg::SRGWorldModel* wm);
    Path(const Path& path);
    ~Path();
    srgsim::Direction getDirection();

    bool isGoalPath();
    int getTotalCosts();
    int getHeuristicCosts();
    std::vector<Path*> expand(std::vector<const srgsim::Cell*>& visited);
    const srgsim::Cell* getCell();
    friend std::ostream& operator<<(std::ostream& os, const srg::robot::Path& path)
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
    Path* addStep(const srgsim::Cell* cell);
    bool checkValidity(std::vector<const srgsim::Cell*>& visited, srgsim::Cell* cell);

    int costs;
    const srgsim::Cell* cell;
    srgsim::Coordinate start;
    srgsim::Coordinate goal;
    Path* lastStep;
};
} // namespace robot
} // namespace srg