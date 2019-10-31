#pragma once

#include <srgsim/world/Cell.h>
#include <srgsim/SRGEnums.h>
#include <srg/SRGWorldModel.h>

namespace srg {
    namespace robot {
        class Path {
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
            std::string toString();

        private:
            Path* addStep(const srgsim::Cell* cell);
            bool checkValidity(std::vector<const srgsim::Cell*>& visited, srgsim::Cell* cell);

            int costs;
            const srgsim::Cell* cell;
            srgsim::Coordinate start;
            srgsim::Coordinate goal;
            Path* lastStep;
        };
    }
}