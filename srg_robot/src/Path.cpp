#include "srg/robot/Path.h"

#include <srgsim/world/World.h>

#include <iostream>
#include <sstream>

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
    std::cout << "Path::getDirection()" << std::endl;
    // find the first cell that is not on the start coordinates
    Path* firstStep = this;
    while (firstStep->lastStep->lastStep != nullptr) {
        firstStep = firstStep->lastStep;
    }
    if (this->start.x < firstStep->cell->coordinate.x) {
        return srgsim::Direction::Right;
    } else if (this->start.x > firstStep->cell->coordinate.x) {
        return srgsim::Direction::Left;
    } else if (this->start.y < firstStep->cell->coordinate.y) {
        return srgsim::Direction::Up;
    } else if (this->start.y > firstStep->cell->coordinate.y) {
        return srgsim::Direction::Down;
    } else {
        std::cerr << "Path::getDirection(): Path is longer than 1 step, but stays on the same coordinates!" << std::endl;
        return srgsim::Direction::None;
    }
}

bool Path::isGoalPath()
{
    return this->cell->coordinate == this->goal;
}

int Path::getCosts()
{
    int costs = this->costs;
    costs += std::abs<int>(this->cell->coordinate.x - this->goal.x);
    costs += std::abs<int>(this->cell->coordinate.y - this->goal.y);
    return costs;
}

std::vector<Path*> Path::expand(std::vector<const srgsim::Cell*>& visited)
{
    std::vector<Path*> newPaths;
    auto entry = std::find(visited.begin(), visited.end(), cell->down);
    if (entry == visited.end()) {
        if (cell->down->type == srgsim::Type::DoorOpen || cell->down->type == srgsim::Type::Floor || cell->down->type == srgsim::Type::DoorClosed) {
            newPaths.push_back(this->addStep(cell->down));
        }
    }

    entry = std::find(visited.begin(), visited.end(), cell->up);
    if (entry == visited.end()) {
        if (cell->up->type == srgsim::Type::DoorOpen || cell->up->type == srgsim::Type::Floor || cell->down->type == srgsim::Type::DoorClosed) {
            newPaths.push_back(this->addStep(cell->up));
        }
    }

    entry = std::find(visited.begin(), visited.end(), cell->left);
    if (entry == visited.end()) {
        if (cell->left->type == srgsim::Type::DoorOpen || cell->left->type == srgsim::Type::Floor || cell->down->type == srgsim::Type::DoorClosed) {
            newPaths.push_back(this->addStep(cell->left));
        }
    }

    entry = std::find(visited.begin(), visited.end(), cell->right);
    if (entry == visited.end()) {
        if (cell->right->type == srgsim::Type::DoorOpen || cell->right->type == srgsim::Type::Floor ||
                cell->down->type == srgsim::Type::DoorClosed) {
            newPaths.push_back(this->addStep(cell->right));
        }
    }
    return newPaths;
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

std::string Path::toString() {
    std::stringstream ss;
    ss << "Path: ";
    Path* currentPath = this;
    while(currentPath->lastStep) {
        ss << "(" << currentPath->cell->coordinate.x << ", " << currentPath->cell->coordinate.y << "), ";
        currentPath = currentPath->lastStep;
    }
    ss << "(" << currentPath->cell->coordinate.x << ", " << currentPath->cell->coordinate.y << "), ";
    return ss.str();
}
} // namespace robot
} // namespace srg
