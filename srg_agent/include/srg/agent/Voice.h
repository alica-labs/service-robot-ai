#pragma once

#include <srg/agent/SpeechActMsg.capnp.h>
#include <srg/agent/containers/SpeechAct.h>
#include <essentials/IdentifierConstPtr.h>

#include <capnzero/Subscriber.h>

namespace capnzero
{
class Publisher;
}

namespace essentials
{
class IDManager;
class SystemConfig;
} // namespace essentials

namespace srg
{
namespace agent
{
class Voice
{
public:
    explicit Voice(bool standalone = false);
    Voice(essentials::IdentifierConstPtr ownId, essentials::IDManager& idManager);

    ~Voice();

    void onSpeechAct(capnp::FlatArrayMessageReader& msg);

    void speak(std::string input);
    void speak(SpeechAct speechAct) const;

private:
    bool parseInput(std::string input, SpeechAct& speechAct);

    std::vector<std::string> split(std::string input);

    essentials::SystemConfig& sc;
    essentials::IDManager* idManager;
    essentials::IdentifierConstPtr id;

    void* ctx;
    capnzero::Publisher* speechActPublisher;
    capnzero::Subscriber* speechActSubscriber;
};
} // namespace human
} // namespace srg