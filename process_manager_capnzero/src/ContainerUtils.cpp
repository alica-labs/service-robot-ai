#include "process_manager/containers/ContainerUtils.h"

#include "process_manager/ProcessCommandMsg.capnp.h"
#include "process_manager/ProcessStatsMsg.capnp.h"
#include "process_manager/ProcessStatMsg.capnp.h"
#include <essentials/IDManager.h>
#include <essentials/WildcardID.h>

namespace process_manager
{

ProcessCommand ContainerUtils::toProcessCommand(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager)
{
    ProcessCommand pc;
    process_manager::ProcessCommandMsg::Reader reader = msg.getRoot<process_manager::ProcessCommandMsg>();
    // check whether this message is for me, 0 is a wild card for all ProcessManagers
    essentials::WildcardID wildcardId(nullptr, 0);
    if ((uint8_t) reader.getSenderID().getType() == essentials::Identifier::WILDCARD_TYPE) {
        pc.receiverID = &wildcardId;
    } else {
        pc.receiverID = idManager->getIDFromBytes(
                reader.getSenderID().getValue().asBytes().begin(), reader.getSenderID().getValue().size(), (uint8_t) reader.getSenderID().getType());
    }

    pc.cmd = reader.getCmd();
    for (::capnzero::ID::Reader id : reader.getRobotIds()) {
        pc.robotIDs.push_back(idManager->getIDFromBytes(id.getValue().asBytes().begin(), id.getValue().size(), (uint8_t) id.getType()));
    }
    for (int32_t paramSet : reader.getParamSets()) {
        pc.paramSets.push_back(paramSet);
    }
    return pc;
}

ProcessStats ContainerUtils::toProcessStats(::capnp::FlatArrayMessageReader& msg) {
    std::cout << "ContainerUtils: toProcessStats(..) not implemented, yet!" << std::endl;
}

void ContainerUtils::toMsg(process_manager::ProcessStats ps, ::capnp::MallocMessageBuilder& builder) {
    process_manager::ProcessStatsMsg::Builder msg = builder.getRoot<process_manager::ProcessStatsMsg>();

    capnzero::ID::Builder senderID = msg.initSenderID();
    senderID.setValue(kj::arrayPtr(ps.ownID->getRaw(), (unsigned int) ps.ownID->getSize()));
    senderID.setType(ps.ownID->getType());
    msg.setSenderID(senderID);

    ::capnp::List<process_manager::ProcessStatMsg>::Builder processStats = msg.initProcessStats(ps.processStats.size());
    for (unsigned int i = 0; i < processStats.size(); i++) {
        process_manager::ProcessStatMsg::Builder psBuilder = processStats[i];
        psBuilder.setCpu(ps.processStats[i].cpu);
        psBuilder.setMem(ps.processStats[i].mem);
        psBuilder.setParamSet(ps.processStats[i].paramSet);
        psBuilder.setProcessKey(ps.processStats[i].processKey);
        psBuilder.setPublishing(ps.processStats[i].publishing);
        psBuilder.setState(ps.processStats[i].state);
        capnzero::ID::Builder robotID = psBuilder.initRobotID();
        robotID.setType(ps.processStats[i].robotID->getType());
        robotID.setValue(::capnp::Data::Reader(ps.processStats[i].robotID->getRaw(), ps.processStats[i].robotID->getSize()));
    }
}

void ContainerUtils::toMsg(process_manager::ProcessCommand pc, ::capnp::MallocMessageBuilder& builder) {
    std::cout << "ContainerUtils: toMsg(process_manager::ProcessCommand pc, ..) not implemented, yet!" << std::endl;
}
} // namespace process_manager