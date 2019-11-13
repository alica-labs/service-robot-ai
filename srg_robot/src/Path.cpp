#include "srg/robot/Path.h"

#include <srgsim/world/World.h>
#include <srgsim/world/Object.h>
#include <srgsim/world/Door.h>
#include <srgsim/world/RoomType.h>

#include <iostream>

namespace srg
{
namespace robot
{
Path::Path(srgsim::Coordinate start, srgsim::Coordinate goal, srg::SRGWorldModel* wm)
        : start(start)
        , goal(goal)
        , lastStep(nullptr)
{
    this->costs = 1;
    this->cell = wm->sRGSimData.getWorld()->getCell(start);
}

Path::Path(const srg::robot::Path& path)
        : start(path.start)
        , goal(path.goal)
        , costs(path.costs)
        , cell(path.cell)
        , lastStep(path.lastStep)
{
}

Path::~Path() {
    delete this->lastStep;
}

srgsim::Direction Path::getDirection()
{
    if (start == goal) {
        return srgsim::Direction::None;
    }
    // find the first cell that is not on the start coordinates
    Path* firstStep = this;
    while (firstStep->lastStep->lastStep != nullptr) {
        firstStep = firstStep->lastStep;
    }
//    std::cout << "Path::getDirection(): First Step is " << firstStep->getCell()->coordinate << " Start is " << this->start <<std::endl;
    if (this->start.x < firstStep->cell->coordinate.x) {
        return srgsim::Direction::Right;
    } else if (this->start.x > firstStep->cell->coordinate.x) {
        return srgsim::Direction::Left;
    } else if (this->start.y < firstStep->cell->coordinate.y) {
        return srgsim::Direction::Down;
    } else if (this->start.y > firstStep->cell->coordinate.y) {
        return srgsim::Direction::Up;
    } else {
        std::cerr << "Path::getDirection(): Path is longer than 1 step, but stays on the same coordinates!" << std::endl;
        return srgsim::Direction::None;
    }
}

bool Path::isGoalPath()
{
    return this->cell->coordinate == this->goal;
}

int Path::getTotalCosts()
{
    return this->costs + this->getHeuristicCosts();
}

int Path::getHeuristicCosts()
{
    return std::abs<int>(this->goal.x - this->cell->coordinate.x) + std::abs<int>(this->goal.y - this->cell->coordinate.y);
}

std::vector<Path*> Path::expand(std::vector<const srgsim::Cell*>& visited)
{
    std::vector<Path*> newPaths;
    if (checkValidity(visited, cell->down)){
        newPaths.push_back(this->addStep(cell->down));
    }
    if (checkValidity(visited, cell->left)){
        newPaths.push_back(this->addStep(cell->left));
    }
    if (checkValidity(visited, cell->right)){
        newPaths.push_back(this->addStep(cell->right));
    }
    if (checkValidity(visited, cell->up)){
        newPaths.push_back(this->addStep(cell->up));
    }
    return newPaths;
}

/**
 * Only allows to step on floor cells without closed doors on it.
 * @param visited
 * @param cell
 * @return
 */
bool Path::checkValidity(std::vector<const srgsim::Cell*>& visited, srgsim::Cell* cell) {
    if (!cell) {
        return false;
    }

    auto entry = std::find(visited.begin(), visited.end(), cell);
    if (entry != visited.end()) {
        return false;
    }

    if (cell->getType() == srgsim::RoomType::Wall) {
        return false;
    }

    for (srgsim::Object* object : cell->getObjects()) {
        if(class srgsim::Door* door = dynamic_cast<class srgsim::Door*>(object)) {
            return door->isOpen();
        }
    }

    return true;
}

const srgsim::Cell* Path::getCell() {
    return this->cell;
}

Path* Path::addStep(const srgsim::Cell* cell)
{
    Path* newPath = new Path(*this);
    newPath->lastStep = this;
    newPath->cell = cell;
    newPath->costs++;
    return newPath;
}
} // namespace robot
} // namespace srg
