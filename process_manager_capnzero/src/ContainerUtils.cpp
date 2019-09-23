#include "process_manager/containers/ContainerUtils.h"

#include "process_manager/ProcessCommandMsg.capnp.h"
#include "process_manager/ProcessStatMsg.capnp.h"
#include "process_manager/ProcessStatsMsg.capnp.h"
#include <essentials/IDManager.h>
#include <essentials/WildcardID.h>

namespace process_manager
{

ProcessCommand ContainerUtils::toProcessCommand(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager)
{
    ProcessCommand pc;
    process_manager::ProcessCommandMsg::Reader reader = msg.getRoot<process_manager::ProcessCommandMsg>();
    pc.receiverID = idManager->getIDFromBytes(
            reader.getSenderID().getValue().asBytes().begin(), reader.getSenderID().getValue().size(), reader.getSenderID().getType());
    pc.cmd = reader.getCmd();
    for (::capnzero::ID::Reader id : reader.getRobotIds()) {
        pc.robotIDs.push_back(idManager->getIDFromBytes(id.getValue().asBytes().begin(), id.getValue().size(), id.getType()));
    }
    for (int32_t paramSet : reader.getParamSets()) {
        pc.paramSets.push_back(paramSet);
    }
    return pc;
}

ProcessStats ContainerUtils::toProcessStats(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager)
{
    ProcessStats psts;
    process_manager::ProcessStatsMsg::Reader reader = msg.getRoot<process_manager::ProcessStatsMsg>();
    psts.senderID = idManager->getIDFromBytes(
            reader.getSenderID().getValue().asBytes().begin(), reader.getSenderID().getValue().size(), reader.getSenderID().getType());
    for (process_manager::ProcessStatMsg::Reader pstMsg : reader.getProcessStats()) {
        process_manager::ProcessStat pst;
        pst.robotID = idManager->getIDFromBytes(
                pstMsg.getRobotID().getValue().asBytes().begin(), pstMsg.getRobotID().getValue().size(), pstMsg.getRobotID().getType());
        pst.state = pstMsg.getState();
        pst.publishing = pstMsg.getPublishing();
        pst.processKey = pstMsg.getProcessKey();
        pst.mem = pstMsg.getMem();
        pst.cpu = pstMsg.getCpu();
        pst.paramSet = pstMsg.getParamSet();
        psts.processStats.push_back(pst);
    }
    return psts;
}

void ContainerUtils::toMsg(process_manager::ProcessStats psts, ::capnp::MallocMessageBuilder& builder)
{
    process_manager::ProcessStatsMsg::Builder msg = builder.getRoot<process_manager::ProcessStatsMsg>();

    capnzero::ID::Builder senderID = msg.initSenderID();
    senderID.setValue(kj::arrayPtr(psts.senderID->getRaw(), (unsigned int) psts.senderID->getSize()));
    senderID.setType(psts.senderID->getType());

    ::capnp::List<process_manager::ProcessStatMsg>::Builder processStats = msg.initProcessStats(psts.processStats.size());
    for (unsigned int i = 0; i < processStats.size(); i++) {
        process_manager::ProcessStatMsg::Builder psBuilder = processStats[i];
        psBuilder.setCpu(psts.processStats[i].cpu);
        psBuilder.setMem(psts.processStats[i].mem);
        psBuilder.setParamSet(psts.processStats[i].paramSet);
        psBuilder.setProcessKey(psts.processStats[i].processKey);
        psBuilder.setPublishing(psts.processStats[i].publishing);
        psBuilder.setState(psts.processStats[i].state);
        capnzero::ID::Builder robotID = psBuilder.initRobotID();
        robotID.setType(psts.processStats[i].robotID->getType());
        robotID.setValue(::capnp::Data::Reader(psts.processStats[i].robotID->getRaw(), psts.processStats[i].robotID->getSize()));
    }
}

void ContainerUtils::toMsg(process_manager::ProcessCommand pc, ::capnp::MallocMessageBuilder& builder)
{
    std::cout << "ContainerUtils: toMsg(process_manager::ProcessCommand pc, ..) not implemented, yet!" << std::endl;
}
} // namespace process_manager