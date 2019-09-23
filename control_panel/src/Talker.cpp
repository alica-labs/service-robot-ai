#include "control/Talker.h"

#include "ui_ControlPanel.h"

#include <control/containers/ContainerUtils.h>
#include <control/ControlPanel.h>
#include <essentials/IDManager.h>
#include <essentials/IdentifierConstPtr.h>
#include <SystemConfig.h>
#include <capnzero/CapnZero.h>
#include <zmq.h>

#include <thread>

#define DEBUG_TALKER

namespace control
{
Talker::Talker(ControlPanel* controlPanel) :
        controlPanel(controlPanel)
{
    QObject::connect(this->controlPanel->uiControlPanel->sendBtn, SIGNAL(clicked(bool)), this, SLOT(run()), Qt::DirectConnection);

    this->sc = essentials::SystemConfig::getInstance();

    this->idManager = new essentials::IDManager();

    int64_t id = (*sc)["ControlPanel"]->get<int64_t>("id", NULL);
    this->id = this->idManager->getID<int64_t>(id);

    this->ctx = zmq_ctx_new();

    this->speechActPublisher = new capnzero::Publisher(this->ctx, capnzero::Protocol::UDP);
    this->speechActPublisher->setDefaultTopic((*sc)["ControlPanel"]->get<std::string>("SpeechAct.topic", NULL));
    this->speechActPublisher->addAddress((*sc)["ControlPanel"]->get<std::string>("SpeechAct.address", NULL));

    this->speechActSubscriber = new capnzero::Subscriber(this->ctx, capnzero::Protocol::UDP);
    this->speechActSubscriber->setTopic((*sc)["ControlPanel"]->get<std::string>("SpeechAct.topic", NULL));
    this->speechActSubscriber->addAddress((*sc)["ControlPanel"]->get<std::string>("SpeechAct.address", NULL));
    this->speechActSubscriber->subscribe(&Talker::onSpeechAct, &(*this));
}

Talker::~Talker()
{
    delete this->speechActPublisher;
    delete this->speechActSubscriber;
    zmq_ctx_term(this->ctx);
}

void Talker::run()
{
    std::string input = this->controlPanel->uiControlPanel->lineEdit->text().toStdString();
    SpeechAct speechAct;
    if (parseInput(input, speechAct)) {
        this->send(speechAct);
    }
}

bool Talker::parseInput(std::string input, SpeechAct& speechAct)
{
    std::cout << "Talker::parseInput(): Input was: '" << input << "'" << std::endl;
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

void Talker::send(SpeechAct speechAct) const
{
    ::capnp::MallocMessageBuilder msgBuilder;
    ContainerUtils::toMsg(speechAct, msgBuilder);
    std::cout << "Talker::send() sending msg '" << msgBuilder.getRoot<SpeechActMsg>().toString().flatten().cStr() << "'" << std::endl;
    this->speechActPublisher->send(msgBuilder);
}

void Talker::onSpeechAct(capnp::FlatArrayMessageReader& msg)
{
    std::cout << "Talker::onSpeechAct(): receiving msg '" << msg.getRoot<SpeechActMsg>().toString().flatten().cStr() << "'" << std::endl;
}

std::vector<std::string> Talker::split(std::string input) {
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
        std::cerr << "Talker::split(): Wrong input! Usage is '[i|r|c] <receiver-id> <arbitrary strings>'" << std::endl;
        return std::vector<std::string>();
    } else {
        return splittedInput;
    }
}
} // namespace control
