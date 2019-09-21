#include "srg/robot/Movement.h"

#include <queue>

namespace srg
{
namespace robot
{
Movement::Movement(srg::SRGWorldModel* wm)
        : wm(wm)
{
}

Path* Movement::searchPath(srgsim::Coordinate start, srgsim::Coordinate goal)
{
    auto cmp = [](Path* left, Path* right) { return (left->getCosts()) > (right->getCosts()); };
    std::priority_queue<Path*, std::vector<Path*>, decltype(cmp)> fringe(cmp);

    Path* initialPath = new Path(start, goal, wm);
    fringe.push(initialPath);
    std::vector<const srgsim::Cell*> visited;

    while (Path* currentPath = fringe.top()) {
        fringe.pop();
//        std::cout << "Movement::searchPath: #1" << std::endl;
//        std::cout << "Movement::searchPath: Current " << currentPath->toString() << std::endl;

        if (currentPath->isGoalPath()) {
//            while (Path* path = fringe.top()) {
//                fringe.pop();
//                delete path;
//            }
            return currentPath;
        }

//        std::cout << "Movement::searchPath: Expanded :" << std::endl;
        std::vector<Path*> newPaths = currentPath->expand(visited);
        if (newPaths.size() == 0) {
//            delete currentPath;
        } else {
            for (auto path : newPaths) {
                fringe.push(path);
                visited.push_back(path->getCell());
//                std::cout << path->toString() << std::endl;
            }
        }
    }
    return initialPath;
}
} // namespace robot
} // namespace srg
