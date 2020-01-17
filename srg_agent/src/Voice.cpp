#include "srg/agent/Voice.h"

#include <SystemConfig.h>
#include <capnzero/CapnZero.h>
#include <essentials/IDManager.h>
#include <essentials/IdentifierConstPtr.h>
#include <srg/agent/containers/ContainerUtils.h>
#include <zmq.h>

#define DEBUG_VOICE

namespace srg
{
namespace agent
{
Voice::Voice(bool standalone)
{
    this->sc = essentials::SystemConfig::getInstance();

    if (standalone) {
        this->idManager = new essentials::IDManager();

        int64_t id = (*sc)["Voice"]->get<int64_t>("defaultID", NULL);
        this->id = this->idManager->getID<int64_t>(id);
    }

    this->ctx = zmq_ctx_new();

    this->speechActPublisher = new capnzero::Publisher(this->ctx, capnzero::Protocol::UDP);
    this->speechActPublisher->setDefaultTopic((*sc)["Voice"]->get<std::string>("SpeechAct.topic", NULL));
    this->speechActPublisher->addAddress((*sc)["Voice"]->get<std::string>("SpeechAct.address", NULL));

    this->speechActSubscriber = new capnzero::Subscriber(this->ctx, capnzero::Protocol::UDP);
    this->speechActSubscriber->setTopic((*sc)["Voice"]->get<std::string>("SpeechAct.topic", NULL));
    this->speechActSubscriber->addAddress((*sc)["Voice"]->get<std::string>("SpeechAct.address", NULL));
    this->speechActSubscriber->subscribe(&Voice::onSpeechAct, &(*this));
}

Voice::Voice(essentials::IdentifierConstPtr ownId, essentials::IDManager* idManager)
        : Voice()
{
    this->idManager = idManager;
    this->id = ownId;
}

Voice::~Voice()
{
    delete this->speechActPublisher;
    delete this->speechActSubscriber;
    zmq_ctx_term(this->ctx);
}

void Voice::speak(std::string input)
{
    SpeechAct speechAct;
    if (parseInput(input, speechAct)) {
        this->speak(speechAct);
    }
}

bool Voice::parseInput(std::string input, SpeechAct& speechAct)
{
    std::cout << "[Voice] Input was: '" << input << "'" << std::endl;
    std::vector<std::string> splittedInput = this->split(input);

    if (splittedInput[0].compare("i") == 0) {
        speechAct.type = SpeechType::inform;
    } else if (splittedInput[0].compare("r") == 0) {
        speechAct.type = SpeechType::request;
    } else if (splittedInput[0].compare("c") == 0) {
        speechAct.type = SpeechType::command;
    } else {
        return false;
    }

    speechAct.senderID = this->id;
    int receiverID = std::stoi(splittedInput[1]);
    speechAct.receiverID = this->idManager->getID(receiverID);
    speechAct.text = splittedInput[2];
    speechAct.actID = this->idManager->generateID(4);
    speechAct.previousActID = this->idManager->getWildcardID();

    return true;
}

void Voice::speak(SpeechAct speechAct) const
{
    ::capnp::MallocMessageBuilder msgBuilder;
    ContainerUtils::toMsg(speechAct, msgBuilder);
//    std::cout << "[Voice] sending msg '" << msgBuilder.getRoot<SpeechActMsg>().toString().flatten().cStr() << "'" << std::endl;
    this->speechActPublisher->send(msgBuilder);
}

void Voice::onSpeechAct(capnp::FlatArrayMessageReader& msg)
{
//    std::cout << "[Voice] receiving msg '" << msg.getRoot<SpeechActMsg>().toString().flatten().cStr() << "'" << std::endl;
}

std::vector<std::string> Voice::split(std::string input)
{
    std::vector<std::string> splittedInput;
    size_t lastIdx = 0;
    size_t curIdx = 0;

    while (splittedInput.size() < 2 && lastIdx < input.size()) {
        curIdx = input.find(' ', lastIdx);
        splittedInput.push_back(input.substr(lastIdx, curIdx - lastIdx));
        lastIdx = curIdx + 1;
    }
    splittedInput.push_back(input.substr(lastIdx));

    if (splittedInput.size() != 3) {
        std::cerr << "[Voice] Wrong input! Usage is '[i|r|c] <receiver-id> <arbitrary strings>'" << std::endl;
        return std::vector<std::string>();
    } else {
        return splittedInput;
    }
}
} // namespace agent
} // namespace srg