#pragma once

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
    Communication(SRGWorldModel* wm);
    virtual ~Communication();

private:
    SRGWorldModel* wm;
    essentials::SystemConfig* sc;

    void* ctx;
    capnzero::Subscriber* telegramMessageSub;
    capnzero::Subscriber* speechActSub;

    void onTelegramMessage(capnp::FlatArrayMessageReader& msg);
    void onSpeechAct(capnp::FlatArrayMessageReader& msg);
};
} // namespace wm
} // namespace srg