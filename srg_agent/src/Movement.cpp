#include "srg/agent/Movement.h"
#include <srg/world/Door.h>
#include <srg/world/RoomType.h>
#include <srg/World.h>

#include <queue>

namespace srg
{
namespace agent
{
Movement::Movement(srg::SRGWorldModel* wm)
        : wm(wm)
{
}

Path* Movement::searchPath(srg::world::Coordinate start, srg::world::Coordinate goal)
{
    if (!checkValidity(goal)) {
        goal = this->findAlternativeGoal(start, goal);
        std::cout << "Movement::searchPath(): Alternative Goal is " << goal << std::endl;
    }

    auto cmp = [](Path* left, Path* right) { return (left->getTotalCosts()) > (right->getTotalCosts()); };
    std::priority_queue<Path*, std::vector<Path*>, decltype(cmp)> fringe(cmp);

    Path* initialPath = new Path(start, goal, wm);
    fringe.push(initialPath);
    std::vector<std::shared_ptr<const world::Cell>> visited;

    while (Path* currentPath = fringe.top()) {
        fringe.pop();
        //        std::cout << "Movement::searchPath: #1" << std::endl;
        //        std::cout << "Movement::searchPath: Current " << currentPath->toString() << std::endl;

        if (currentPath->isGoalPath()) {
            return currentPath;
        }

        if (currentPath->getHeuristicCosts() < initialPath->getHeuristicCosts()) {
            initialPath = currentPath;
        }

        //        std::cout << "Movement::searchPath: Expanded :" << std::endl;
        std::vector<Path*> newPaths = currentPath->expand(visited);
        for (auto path : newPaths) {
            fringe.push(path);
            visited.push_back(path->getCell());
            //                std::cout << path->toString() << std::endl;
        }
    }
    return initialPath;
}

    srg::world::Coordinate Movement::findAlternativeGoal(srg::world::Coordinate start, srg::world::Coordinate goal)
{
    int32_t deltaX = (start.x - goal.x > 0 ? 1 : -1);
    int32_t deltaY = (start.y - goal.y > 0 ? 1 : -1);

    srg::world::Coordinate alternativeGoal = goal;
    alternativeGoal.x += deltaX;
    alternativeGoal.y += deltaY;
    if (checkValidity(alternativeGoal)) {
        return alternativeGoal;
    }

    alternativeGoal = goal;
    alternativeGoal.x += deltaX;
    if (checkValidity(alternativeGoal)) {
        return alternativeGoal;
    }

    alternativeGoal = goal;
    alternativeGoal.y += deltaY;
    if (checkValidity(alternativeGoal)) {
        return alternativeGoal;
    }

    alternativeGoal = goal;
    alternativeGoal.x -= deltaX;
    alternativeGoal.y += deltaY;
    if (checkValidity(alternativeGoal)) {
        return alternativeGoal;
    }

    alternativeGoal = goal;
    alternativeGoal.x += deltaX;
    alternativeGoal.y -= deltaY;
    if (checkValidity(alternativeGoal)) {
        return alternativeGoal;
    }

    alternativeGoal = goal;
    alternativeGoal.y -= deltaY;
    if (checkValidity(alternativeGoal)) {
        return alternativeGoal;
    }

    alternativeGoal = goal;
    alternativeGoal.x -= deltaX;
    if (checkValidity(alternativeGoal)) {
        return alternativeGoal;
    }

    alternativeGoal = goal;
    alternativeGoal.x -= deltaX;
    alternativeGoal.y -= deltaY;
    if (checkValidity(alternativeGoal)) {
        return alternativeGoal;
    }

    std::cerr << "Movement::findAlternativeGoal(): No alternative found for Goal " << goal << std::endl;
    return goal;
}

bool Movement::checkValidity(srg::world::Coordinate coord)
{
    std::shared_ptr<const world::Cell> cell = wm->sRGSimData.getWorld()->getCell(coord);
    if (cell->getType() == srg::world::RoomType::Wall) {
        return false;
    }

    for (auto& objectEntry : cell->getObjects()) {
        if (std::shared_ptr<srg::world::Door> door = std::dynamic_pointer_cast<srg::world::Door>(objectEntry.second)) {
            return door->isOpen();
        }
    }

    return true;
}
} // namespace agent
} // namespace srg
