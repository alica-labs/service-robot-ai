@0xae0df4f534e8b04f;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("process_manager");

using IDMsg = import "/capnzero/ID.capnp";

struct ProcessCommandMsg {
  senderID @0 :IDMsg.ID;

  # Cmd has to be set to one of the values above.
  cmd @1 :UInt8;

  # ReceiverId determines the ProcessManager-Instance, which is addressed by this message. 0 means every ProcessManager
  receiverID @2 :IDMsg.ID;

  # RobotIds determines the ROBOT-Environment Variable.
  robotIds @3 :List(IDMsg.ID);

  # List of process IDs from the Processes.conf file.
  processKeys @4 :List(Int32);

  # List of paramSet IDs from the Processes.conf file.
  paramSets @5 :List(Int32);
}
