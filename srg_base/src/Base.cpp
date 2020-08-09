#include "Base.h"

#include <BehaviourCreator.h>
#include <ConditionCreator.h>
#include <ConstraintCreator.h>
#include <UtilityFunctionCreator.h>
#include <alica_capnzero_proxy/Communication.h>
#include <asp_solver_wrapper/ASPSolverWrapper.h>
#include <engine/AlicaContext.h>
#include <reasoner/asp/Solver.h>
#include <srg/SRGWorldModel.h>
#include <essentials/Identifier.h>
#include <essentials/IDManager.h>

#include <chrono>
#include <csignal>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

namespace srg
{

bool Base::running = false;

Base::Base(std::string roleSetName, std::string masterPlanName, std::string roleSetDir, essentials::IdentifierConstPtr identifier)
        : ac(new alica::AlicaContext(roleSetName, masterPlanName, false, identifier))
{
    ac->setCommunicator<alica_capnzero_proxy::Communication>();

    // ASP Solver
    ac->addSolver<alica::reasoner::ASPSolverWrapper>();
    auto solver = new ::reasoner::asp::Solver({});
    ac->getSolver<alica::reasoner::ASPSolverWrapper>().init(solver);

    wm = SRGWorldModel::getInstance();
    wm->setAlicaContext(ac);
    wm->setSolver(solver);
    wm->init();
}

void Base::start()
{
    running = true;
    alica::AlicaCreators creators(std::make_unique<alica::ConditionCreator>(), std::make_unique<alica::UtilityFunctionCreator>(),
            std::make_unique<alica::ConstraintCreator>(), std::make_unique<alica::BehaviourCreator>());
    if (ac->init(creators) != 0) {
        std::cerr << "Base: Unable to initialize the Alica Engine successfull!" << std::endl;
    }
}

Base::~Base()
{
    ac->terminate();
    delete ac;
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
    int id = essentials::SystemConfig::getInstance().getOwnRobotID();
    essentials::IDManager* idManager = new essentials::IDManager();
    essentials::IdentifierConstPtr agentId = idManager->getID(id);
    srg::Base* base = new srg::Base(roleset, masterplan, rolesetdir, agentId);

    signal(SIGINT, srg::Base::simSigintHandler);

    std::cout << "\nStarting Base ..." << std::endl;
    base->start();

    while (base->isRunning()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    delete idManager;
    delete base;

    return 0;
}
