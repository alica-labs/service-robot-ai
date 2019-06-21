@0xa9093f7144c0408e;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("process_manager");

using IDMsg = import "/capnzero/ID.capnp";

struct ProcessStatMsg {
  # RobotId determines the ROBOT-Environment Variables, under which the reported process is running.
  robotID @0 :IDMsg.ID;

  # Process ID from the Processes.conf file.
  processKey @1 :Int32;

  # Parameter Set ID from the Processes.conf file
  paramSet @2 :Int32;

  # State has to be set to one of the values above.
  state @3 :Int8;

  # cpu is the percentage of required cpu resources of this process (we need uint16 as one process sometimes needs more than 255 %)
  cpu @4 :UInt16;

  # mem is the amount of MB this process needs.
  mem @5 :UInt32;

  # flag whether logs are published
  publishing @6 :UInt8;

  # constants for publishing
  const publishingOff :UInt8 = 0;
  const publishingOn :UInt8 = 1;
}






