@0xd6da624da6f5faee;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("control");

using IDMsg = import "/capnzero/ID.capnp";

struct AgentCommandMsg{

    # ReceiverID 0 is reserved as wildcard for all robots. If it is > 0, it represents the ID of the robot.
    receiverId @0 :IDMsg.ID;

    cmd @1 :UInt8;

    # The following commands are defined
    const start :UInt8 = 0;
    const stop :UInt8 = 1;
}