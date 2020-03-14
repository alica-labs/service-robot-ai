#pragma once

#include <srg/agent/containers/SpeechAct.h>

#include <SystemConfig.h>
#include <capnzero/CapnZero.h>

namespace srg
{
class SRGWorldModel;

namespace wm
{
class Communication
{
public:
    explicit Communication(SRGWorldModel* wm);
    virtual ~Communication();

    void sendSpeechAct(std::shared_ptr<const srg::agent::SpeechAct> speechAct) const;

private:
    SRGWorldModel* wm;
    essentials::SystemConfig* sc;

    void* ctx;
    capnzero::Subscriber* speechActSub;
    capnzero::Publisher* speechActPub;
    capnzero::Subscriber* agentCommandSub;
    capnzero::Subscriber* perceptionSub;

    void onSpeechAct(capnp::FlatArrayMessageReader& msg);
    void onAgentCmd(capnp::FlatArrayMessageReader& msg);
    void onSimPerceptions(capnp::FlatArrayMessageReader& msg);
};
} // namespace wm
} // namespace srg