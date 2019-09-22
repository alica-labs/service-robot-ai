@0xa89f822a2978bd4a;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("control");

using IDMsg = import "/capnzero/ID.capnp";

struct SpeechActMsg {
  senderID @0 :IDMsg.ID;
  receiverID @1 :IDMsg.ID;
  actID @2 :IDMsg.ID;
  previousActID @3 :IDMsg.ID;
  text @4 :Text;
  speechType @5 :UInt16;
}

