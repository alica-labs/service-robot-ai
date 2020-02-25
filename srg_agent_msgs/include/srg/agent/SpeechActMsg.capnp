@0xa89f822a2978bd4a;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("srg::agent");

using IDMsg = import "/capnzero/ID.capnp";
using PMsg = import "/srg/sim/msgs/PerceptionMsg.capnp";

struct SpeechActMsg {
  senderID @0 :IDMsg.ID;
  receiverID @1 :IDMsg.ID;
  actID @2 :IDMsg.ID;
  previousActID @3 :IDMsg.ID;
  text @4 :Text;
  speechType @5 :UInt16;
  objectRequestType @6 :PMsg.PerceptionMsg.Object.Type;
  perception @7 :PMsg.PerceptionMsg;
}

