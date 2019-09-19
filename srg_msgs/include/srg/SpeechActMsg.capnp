@0xa89f822a2978bd4a;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("srg");

using IDMsg = import "/capnzero/ID.capnp";

struct SpeechActMsg {
  senderID @0 :IDMsg.ID;
  actID @1 :IDMsg.ID;
  previousActID @2 :IDMsg.ID;
  text @3 :Text;
  speechType @4 :UInt16;
}

