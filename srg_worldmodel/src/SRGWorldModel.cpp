#include "srg/SRGWorldModel.h"

#include "srg/conceptnet/ConceptNet.h"
#include "srg/asp/SRGKnowledgeManager.h"
#include "srg/asp/ASPTranslator.h"

#include <engine/AlicaContext.h>

namespace srg
{

SRGWorldModel* SRGWorldModel::getInstance()
{
    static SRGWorldModel instance;
    return &instance;
}

SRGWorldModel::SRGWorldModel()
        : WorldModel()
        , sRGSimData(this)
        , rawSensorData(this)
        , dialogueManager(this)
        , communication(nullptr)
        , conceptNet(new conceptnet::ConceptNet(this))
        , aspTranslator(new srg::asp::ASPTranslator(this))
        , srgKnowledgeManager(new srg::asp::SRGKnowledgeManager())
        , gui(nullptr)
        , dataProcessingRunning(false)
{
}

SRGWorldModel::~SRGWorldModel()
{
    this->dataProcessingRunning = false;
    this->dataProcessingThread->join();
    delete this->dataProcessingThread;

    delete this->conceptNet;
    delete this->srgKnowledgeManager;
    delete this->aspTranslator;
    delete this->communication;
    delete this->gui;
}

void SRGWorldModel::init()
{
    this->sRGSimData.init();
    this->gui = new srg::GUI(this->getAlicaContext()->getLocalAgentName());
    this->dataProcessingRunning = true;
    this->dataProcessingThread = new std::thread(&SRGWorldModel::run, this);
    this->communication = new wm::Communication(this);
}

/**
 * Must be called before init().
 * @param solver
 */
void SRGWorldModel::setSolver(reasoner::asp::Solver* solver) {
    this->srgKnowledgeManager->setSolver(solver);
}

void SRGWorldModel::run() {
    int interval = 30; // in ms
    while(this->dataProcessingRunning) {

        auto start = std::chrono::system_clock::now();

        // 1. UNQUEUE AND PROCESS MESSAGES

        // SpeechActs for receiving commands, information, and requests
        const std::shared_ptr<const supplementary::InformationElement<agent::SpeechAct>> speechActInfo = this->rawSensorData.editSpeechActBuffer().popLast();
        if (speechActInfo != nullptr) {
            this->dialogueManager.processSpeechAct(speechActInfo);
        }

        // Perceptions for receiving information from the simulator
        const std::shared_ptr<const supplementary::InformationElement<srg::sim::containers::Perceptions>> perceptionsInfo = this->rawSensorData.editPerceptionsBuffer().popLast();
        if (perceptionsInfo != nullptr) {
            this->sRGSimData.processPerception(perceptionsInfo->getInformation());
        }

        // 2. TRIGGER SUBSEQUENT CALCULATIONS AND ACTIONS
        this->dialogueManager.tick();

        // Sleep in order to keep the cpu effort low
        auto timePassed = std::chrono::system_clock::now() - start;
        std::chrono::milliseconds millisecondsPassed = std::chrono::duration_cast<std::chrono::milliseconds>(timePassed);

        if (millisecondsPassed.count() < interval) {
            std::this_thread::sleep_for(std::chrono::milliseconds(interval - millisecondsPassed.count()));
        } else {
            std::cerr << "[SRGWorldModel] Iteration took too long!!! (" << millisecondsPassed.count() << "ms)" << std::endl;
        }
    }
}
} /* namespace wumpus */
