#include "srg/Talker.h"

#include <essentials/IDManager.h>
#include <essentials/IdentifierConstPtr.h>

#include <capnzero/CapnZero.h>
#include <SystemConfig.h>

#include <zmq.h>

#include <chrono>
#include <thread>
#include <signal.h>

#define DEBUG_TALKER

namespace srg {

    bool Talker::operating = true;

    Talker::Talker() : Worker("Dude") {
        this->sc = essentials::SystemConfig::getInstance();

        this->idManager = new essentials::IDManager();

        int64_t id = (*sc)["Talker"]->get<int64_t>("id", NULL);
        this->id = this->idManager->getID<int64_t>(id);

        this->ctx = zmq_ctx_new();
        this->speechActPublisher = new capnzero::Publisher(this->ctx, capnzero::Protocol::UDP);

        this->topic = (*sc)["SRGWorldModel"]->get<std::string>("Data.SpeechAct.Topic", NULL);
        this->speechActPublisher->setDefaultTopic(this->topic);

        this->url = (*sc)["Talker"]->get<std::string>("Communication.URL", NULL);
        this->speechActPublisher->addAddress(this->url);
    }

    Talker::~Talker() {
        delete this->speechActPublisher;

        // Delete zmq context:
        zmq_ctx_term(this->ctx);
    }

    void Talker::s_signal_handler(int signal_value)
    {
        operating = false;
    }

    void Talker::run() {
        std::string input;
        std::getline(std::cin, input);
        std::cout << "Talker: Input was: '" << input << "'" << std::endl;
        this->send(srg::SpeechType::INFORM, input);
    }

    void Talker::send(srg::SpeechType speechType, const std::string& text) const
    {
        ::capnp::MallocMessageBuilder msgBuilder;
        srg::SpeechAct::Builder msg = msgBuilder.initRoot<srg::SpeechAct>();
        capnzero::ID::Builder sender = msg.initSenderID();
        sender.setValue(kj::arrayPtr(this->id->getRaw(), this->id->getSize()));
        sender.setType(capnzero::ID::UUID);
        msg.setSenderID(sender);
        msg.setSpeechType(speechType);
        msg.setText(text);
        if (Talker::operating) {
            std::cout << "Talker: sending msg '" << msg.toString().flatten().cStr() << "'" << std::endl;
            this->speechActPublisher->send(msgBuilder);
        }
    }

}

static void s_catch_signals(void)
{
    struct sigaction action;
    action.sa_handler = srg::Talker::s_signal_handler;
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
    while (srg::Talker::operating) {
        sleep(1);
    }

    talker->stop();

    delete talker;
    return 0;
}
