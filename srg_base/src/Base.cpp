#include "Base.h"

#include <engine/AlicaEngine.h>
#include <BehaviourCreator.h>
#include <ConditionCreator.h>
#include <UtilityFunctionCreator.h>
#include <ConstraintCreator.h>
#include <alica_capnzero_proxy/Communication.h>
#include <srg/SRGWorldModel.h>
#include <essentials/IDManager.h>

#include <asp_solver_wrapper/ASPSolverWrapper.h>
#include <reasoner/asp/Solver.h>

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <csignal>

namespace srg
{

bool Base::running = false;

Base::Base(std::string roleSetName, std::string masterPlanName, std::string roleSetDir)
{
    ae = new alica::AlicaEngine(new essentials::IDManager(), roleSetName, masterPlanName, false);
    bc = new alica::BehaviourCreator();
    cc = new alica::ConditionCreator();
    uc = new alica::UtilityFunctionCreator();
    crc = new alica::ConstraintCreator();

    ae->setAlicaClock(new alica::AlicaClock());
    ae->setCommunicator(new alica_capnzero_proxy::Communication(ae));

    // ASP Solver
//    std::vector<char const*> args{"clingo", nullptr};
    auto solver = new ::reasoner::asp::Solver({});
    auto solverWrapper = new alica::reasoner::ASPSolverWrapper(ae, {});
    solverWrapper->init(solver);
    ae->addSolver(solverWrapper);

    wm = SRGWorldModel::getInstance();
    wm->setEngine(ae);
    wm->init();
    wm->setSolver(solver);

    if (!ae->init(bc, cc, uc, crc)) {
        std::cerr << "Base: Unable to initialize the Alica Engine successfull!" << std::endl;
    }

}

void Base::start()
{
    running = true;
    ae->start();
}

Base::~Base()
{
    ae->shutdown();
    delete ae;
    delete cc;
    delete bc;
    delete uc;
    delete crc;
}

bool Base::isRunning()
{
    return running;
}

/**
 * This is for handling [Ctrl] + [c]
 * @param sig
 */
void Base::simSigintHandler(int sig)
{
    std::cout << "Base: Caught SIGINT! Terminating ..." << std::endl;
    running = false;
}

} // namespace srg

void printUsage()
{
    std::cout << "Usage: ./srg-base -m \"Masterplan\" [-rd \"RoleSetDirectory\"] [-rset \"RoleSet\"]" << std::endl;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        printUsage();
        return 0;
    }

    std::cout << "Parsing command line parameters:" << std::endl;

    std::string masterplan = "";
    std::string rolesetdir = ".";
    std::string roleset = "";

    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-m" || std::string(argv[i]) == "-masterplan") {
            masterplan = argv[i + 1];
            i++;
        }

        if (std::string(argv[i]) == "-rd" || std::string(argv[i]) == "-rolesetdir") {
            rolesetdir = argv[i + 1];
            i++;
        }
        if (std::string(argv[i]) == "-r" || std::string(argv[i]) == "-roleset") {
            roleset = argv[i + 1];
            i++;
        }
    }
    if (masterplan.size() == 0 || rolesetdir.size() == 0) {
        printUsage();
        return 0;
    }
    std::cout << "\tMasterplan is:       \"" << masterplan << "\"" << std::endl;
    std::cout << "\tRolset Directory is: \"" << rolesetdir << "\"" << std::endl;
    std::cout << "\tRolset is:           \"" << (roleset.empty() ? "Default" : roleset) << "\"" << std::endl;

    std::cout << "\nConstructing Base ..." << std::endl;
    srg::Base* base = new srg::Base(roleset, masterplan, rolesetdir);

    signal(SIGINT, srg::Base::simSigintHandler);

    std::cout << "\nStarting Base ..." << std::endl;
    base->start();

    while (base->isRunning()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    delete base;

    return 0;
}
