// Generated by Cap'n Proto compiler, DO NOT EDIT
// source: RobotCommandMsg.capnp

#ifndef CAPNP_INCLUDED_d6da624da6f5faee_
#define CAPNP_INCLUDED_d6da624da6f5faee_

#include <capnp/generated-header-support.h>

#if CAPNP_VERSION != 6001
#error "Version mismatch between generated code and library headers.  You must use the same version of the Cap'n Proto compiler and library."
#endif

#include <capnzero/ID.capnp.h>

namespace capnp {
namespace schemas {

CAPNP_DECLARE_SCHEMA(d2a5711d7217a51b);
CAPNP_DECLARE_SCHEMA(d2cc777e608a0005);
CAPNP_DECLARE_SCHEMA(ed77d9ddb3816e93);

}  // namespace schemas
}  // namespace capnp

namespace robot_control {

struct RobotCommandMsg {
  RobotCommandMsg() = delete;

  class Reader;
  class Builder;
  class Pipeline;
  static constexpr  ::uint8_t START = 0u;
  static constexpr  ::uint8_t STOP = 1u;

  struct _capnpPrivate {
    CAPNP_DECLARE_STRUCT_HEADER(d2a5711d7217a51b, 1, 1)
    #if !CAPNP_LITE
    static constexpr ::capnp::_::RawBrandedSchema const* brand() { return &schema->defaultBrand; }
    #endif  // !CAPNP_LITE
  };
};

// =======================================================================================

class RobotCommandMsg::Reader {
public:
  typedef RobotCommandMsg Reads;

  Reader() = default;
  inline explicit Reader(::capnp::_::StructReader base): _reader(base) {}

  inline ::capnp::MessageSize totalSize() const {
    return _reader.totalSize().asPublic();
  }

#if !CAPNP_LITE
  inline ::kj::StringTree toString() const {
    return ::capnp::_::structString(_reader, *_capnpPrivate::brand());
  }
#endif  // !CAPNP_LITE

  inline bool hasReceiverId() const;
  inline  ::capnzero::ID::Reader getReceiverId() const;

  inline  ::uint8_t getCmd() const;

private:
  ::capnp::_::StructReader _reader;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::_::PointerHelpers;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::List;
  friend class ::capnp::MessageBuilder;
  friend class ::capnp::Orphanage;
};

class RobotCommandMsg::Builder {
public:
  typedef RobotCommandMsg Builds;

  Builder() = delete;  // Deleted to discourage incorrect usage.
                       // You can explicitly initialize to nullptr instead.
  inline Builder(decltype(nullptr)) {}
  inline explicit Builder(::capnp::_::StructBuilder base): _builder(base) {}
  inline operator Reader() const { return Reader(_builder.asReader()); }
  inline Reader asReader() const { return *this; }

  inline ::capnp::MessageSize totalSize() const { return asReader().totalSize(); }
#if !CAPNP_LITE
  inline ::kj::StringTree toString() const { return asReader().toString(); }
#endif  // !CAPNP_LITE

  inline bool hasReceiverId();
  inline  ::capnzero::ID::Builder getReceiverId();
  inline void setReceiverId( ::capnzero::ID::Reader value);
  inline  ::capnzero::ID::Builder initReceiverId();
  inline void adoptReceiverId(::capnp::Orphan< ::capnzero::ID>&& value);
  inline ::capnp::Orphan< ::capnzero::ID> disownReceiverId();

  inline  ::uint8_t getCmd();
  inline void setCmd( ::uint8_t value);

private:
  ::capnp::_::StructBuilder _builder;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
  friend class ::capnp::Orphanage;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::_::PointerHelpers;
};

#if !CAPNP_LITE
class RobotCommandMsg::Pipeline {
public:
  typedef RobotCommandMsg Pipelines;

  inline Pipeline(decltype(nullptr)): _typeless(nullptr) {}
  inline explicit Pipeline(::capnp::AnyPointer::Pipeline&& typeless)
      : _typeless(kj::mv(typeless)) {}

  inline  ::capnzero::ID::Pipeline getReceiverId();
private:
  ::capnp::AnyPointer::Pipeline _typeless;
  friend class ::capnp::PipelineHook;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
};
#endif  // !CAPNP_LITE

// =======================================================================================

inline bool RobotCommandMsg::Reader::hasReceiverId() const {
  return !_reader.getPointerField(
      ::capnp::bounded<0>() * ::capnp::POINTERS).isNull();
}
inline bool RobotCommandMsg::Builder::hasReceiverId() {
  return !_builder.getPointerField(
      ::capnp::bounded<0>() * ::capnp::POINTERS).isNull();
}
inline  ::capnzero::ID::Reader RobotCommandMsg::Reader::getReceiverId() const {
  return ::capnp::_::PointerHelpers< ::capnzero::ID>::get(_reader.getPointerField(
      ::capnp::bounded<0>() * ::capnp::POINTERS));
}
inline  ::capnzero::ID::Builder RobotCommandMsg::Builder::getReceiverId() {
  return ::capnp::_::PointerHelpers< ::capnzero::ID>::get(_builder.getPointerField(
      ::capnp::bounded<0>() * ::capnp::POINTERS));
}
#if !CAPNP_LITE
inline  ::capnzero::ID::Pipeline RobotCommandMsg::Pipeline::getReceiverId() {
  return  ::capnzero::ID::Pipeline(_typeless.getPointerField(0));
}
#endif  // !CAPNP_LITE
inline void RobotCommandMsg::Builder::setReceiverId( ::capnzero::ID::Reader value) {
  ::capnp::_::PointerHelpers< ::capnzero::ID>::set(_builder.getPointerField(
      ::capnp::bounded<0>() * ::capnp::POINTERS), value);
}
inline  ::capnzero::ID::Builder RobotCommandMsg::Builder::initReceiverId() {
  return ::capnp::_::PointerHelpers< ::capnzero::ID>::init(_builder.getPointerField(
      ::capnp::bounded<0>() * ::capnp::POINTERS));
}
inline void RobotCommandMsg::Builder::adoptReceiverId(
    ::capnp::Orphan< ::capnzero::ID>&& value) {
  ::capnp::_::PointerHelpers< ::capnzero::ID>::adopt(_builder.getPointerField(
      ::capnp::bounded<0>() * ::capnp::POINTERS), kj::mv(value));
}
inline ::capnp::Orphan< ::capnzero::ID> RobotCommandMsg::Builder::disownReceiverId() {
  return ::capnp::_::PointerHelpers< ::capnzero::ID>::disown(_builder.getPointerField(
      ::capnp::bounded<0>() * ::capnp::POINTERS));
}

inline  ::uint8_t RobotCommandMsg::Reader::getCmd() const {
  return _reader.getDataField< ::uint8_t>(
      ::capnp::bounded<0>() * ::capnp::ELEMENTS);
}

inline  ::uint8_t RobotCommandMsg::Builder::getCmd() {
  return _builder.getDataField< ::uint8_t>(
      ::capnp::bounded<0>() * ::capnp::ELEMENTS);
}
inline void RobotCommandMsg::Builder::setCmd( ::uint8_t value) {
  _builder.setDataField< ::uint8_t>(
      ::capnp::bounded<0>() * ::capnp::ELEMENTS, value);
}

}  // namespace

#endif  // CAPNP_INCLUDED_d6da624da6f5faee_