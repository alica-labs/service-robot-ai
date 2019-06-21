@0xb49754e6832d5333;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("process_manager");

using IDMsg = import "/capnzero/ID.capnp";
using ProcessStatMsg = import "ProcessStatMsg.capnp";

struct ProcessStatsMsg {
  # SenderId determines the system, the sender of this message is running on.
  # Example: If a process manager is running on the robot nase this is set to 9
  senderID @0 :IDMsg.ID;

  # ProcessStats is a list of statistical information about a specific process, running
  # under a specific ROBOT-Environment variable / on a specific robot
  processStats @1 :List(ProcessStatMsg.ProcessStatMsg);
}
