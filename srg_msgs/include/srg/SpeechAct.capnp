@0xa89f822a2978bd4a;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("srg");

using IDMsg = import "/capnzero/ID.capnp";

struct SpeechAct {
  senderID @0 :IDMsg.ID;
  text @1 :Text;
  speechType @2 :SpeechType;
}

enum SpeechType {
  inform @0;
  request @1;
  command @2;
}