#include "robot_control/containers/ContainerUtils.h"

#include "robot_control/RobotCommandMsg.capnp.h"
#include <essentials/IDManager.h>
#include <essentials/WildcardID.h>

namespace robot_control
{
RobotCommand ContainerUtils::toRobotCommand(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager)
{
    RobotCommand rc;
    //        process_manager::ProcessCommandMsg::Reader reader = msg.getRoot<process_manager::ProcessCommandMsg>();
    //        // check whether this message is for me, 0 is a wild card for all ProcessManagers
    //        essentials::WildcardID wildcardId(nullptr, 0);
    //        if ((uint8_t) reader.getSenderID().getType() == essentials::Identifier::WILDCARD_TYPE) {
    //            pc.receiverID = &wildcardId;
    //        } else {
    //            pc.receiverID = idManager->getIDFromBytes(
    //                    reader.getSenderID().getValue().asBytes().begin(), reader.getSenderID().getValue().size(), (uint8_t) reader.getSenderID().getType());
    //        }
    //
    //        pc.cmd = reader.getCmd();
    //        for (::capnzero::ID::Reader id : reader.getRobotIds()) {
    //            pc.robotIDs.push_back(idManager->getIDFromBytes(id.getValue().asBytes().begin(), id.getValue().size(), (uint8_t) id.getType()));
    //        }
    //        for (int32_t paramSet : reader.getParamSets()) {
    //            pc.paramSets.push_back(paramSet);
    //        }
    return rc;
}

void ContainerUtils::toMsg(robot_control::RobotCommand rc, ::capnp::MallocMessageBuilder& builder)
{
    robot_control::RobotCommandMsg::Builder msg = builder.getRoot<robot_control::RobotCommandMsg>();

    capnzero::ID::Builder receiverID = msg.initReceiverId();
    receiverID.setValue(kj::arrayPtr(rc.receiverID->getRaw(), (unsigned int) rc.receiverID->getSize()));
    receiverID.setType(rc.receiverID->getType());
    msg.setReceiverId(receiverID);
    msg.setCmd(rc.cmd);
}

} // namespace robot_control