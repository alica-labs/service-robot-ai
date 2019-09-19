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

    SpeechAct* speechAct = nullptr;

    if (input[0] == 'i') {
        speechAct->type = SpeechType::inform;
    } else if (input[0] == 'r') {
        speechAct->type = SpeechType::request;
    } else if (input[0] == 'c') {
        speechAct->type = SpeechType::command;
    } else {
        return nullptr;
    }

    speechAct->text = input.substr(2, input.size() - 2);
    speechAct->senderID = this->id;
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
