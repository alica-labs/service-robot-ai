#include "srg/Talker.h"

#include <essentials/IDManager.h>
#include <essentials/IdentifierConstPtr.h>

#include <SystemConfig.h>
#include <capnzero/CapnZero.h>

#include <zmq.h>

#include <chrono>
#include <signal.h>
#include <srg/containers/ContainerUtils.h>
#include <thread>

#define DEBUG_TALKER

namespace srg
{
Talker::Talker()
        : Worker("Dude")
{
    this->sc = essentials::SystemConfig::getInstance();

    this->idManager = new essentials::IDManager();

    int64_t id = (*sc)["Talker"]->get<int64_t>("id", NULL);
    this->id = this->idManager->getID<int64_t>(id);

    this->ctx = zmq_ctx_new();

    this->speechActPublisher = new capnzero::Publisher(this->ctx, capnzero::Protocol::UDP);
    this->speechActPublisher->setDefaultTopic((*sc)["Talker"]->get<std::string>("topic", NULL));
    this->speechActPublisher->addAddress((*sc)["Talker"]->get<std::string>("address", NULL));

    this->speechActSubscriber = new capnzero::Subscriber(this->ctx, capnzero::Protocol::UDP);
    this->speechActSubscriber->setTopic((*sc)["Talker"]->get<std::string>("topic", NULL));
    this->speechActSubscriber->addAddress((*sc)["Talker"]->get<std::string>("address", NULL));
    this->speechActSubscriber->subscribe(&Talker::onSpeechAct, &(*this));
}

Talker::~Talker()
{
    delete this->speechActPublisher;
    zmq_ctx_term(this->ctx);
}

void Talker::run()
{
    std::string input;
    std::getline(std::cin, input);
    SpeechAct* speechAct = parseInput(input);
    if (speechAct) {
        this->send(speechAct);
        delete speechAct;
    }
}

SpeechAct* Talker::parseInput(std::string input)
{
    std::cout << "Talker::parseInput(): Input was: '" << input << "'" << std::endl;
    std::vector<std::string> splittedInput = this->split(input);
    for (std::string splitPart : splittedInput) {
        std::cout << "Talker::parseInput(): Part: '" << splitPart << "'" << std::endl;
    }

    SpeechAct* speechAct = nullptr;
    speechAct->senderID = this->id;
    if (splittedInput[0].compare("i") == 0) {
        speechAct->type = SpeechType::inform;
    } else if (splittedInput[0].compare("r") == 0) {
        speechAct->type = SpeechType::request;
    } else if (splittedInput[0].compare("c") == 0) {
        speechAct->type = SpeechType::command;
    } else {
        return nullptr;
    }
    int receiverID = std::stoi(splittedInput[1]);
    speechAct->receiverID = this->idManager->getID(receiverID);
    speechAct->text = splittedInput[2];
    speechAct->actID = this->idManager->generateID();
    return speechAct;
}

void Talker::send(SpeechAct* speechAct) const
{
    ::capnp::MallocMessageBuilder msgBuilder;
    srg::ContainerUtils::toMsg(*speechAct, msgBuilder);
    if (this->isRunning()) {
        std::cout << "Talker: sending msg '" << msgBuilder.getRoot<srg::SpeechActMsg>().toString().flatten().cStr() << "'" << std::endl;
        this->speechActPublisher->send(msgBuilder);
    }
}

void Talker::onSpeechAct(capnp::FlatArrayMessageReader& msg)
{
    std::cout << "Talker:: receiving msg '" << msg.getRoot<srg::SpeechActMsg>().toString().flatten().cStr() << "'" << std::endl;
}

std::vector<std::string> Talker::split(std::string input) {
    std::vector<std::string> splittedInput;
    size_t lastIdx = 0;
    size_t curIdx = input.find(" ", lastIdx, input.size()-lastIdx);
    while(curIdx != std::string::npos) {
        splittedInput.push_back(input.substr(lastIdx,curIdx-lastIdx));
        lastIdx = curIdx;
        curIdx = input.find(" ", lastIdx, input.size()-lastIdx);
    }
    splittedInput.push_back(input.substr(lastIdx,curIdx-lastIdx));
    return splittedInput;
}
} // namespace srg

static bool operating = true;
static void s_signal_handler(int signal_value)
{
    ::operating = false;
}
static void s_catch_signals(void)
{
    struct sigaction action;
    action.sa_handler = s_signal_handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
}

int main(int argc, char** argv)
{
    s_catch_signals();

    srg::Talker* talker = new srg::Talker();
    talker->setDelayedStartMS(std::chrono::milliseconds(0));
    talker->setIntervalMS(std::chrono::milliseconds(1000));
    talker->start();

    // start running
    while (operating) {
        sleep(1);
    }

    talker->stop();

    delete talker;
    return 0;
}
