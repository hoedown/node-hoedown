#ifndef NODE_HOEDOWN_COMMON_HPP
#define NODE_HOEDOWN_COMMON_HPP

#include "v8u/v8u.hpp"
using namespace v8;

#define NODE_HOEDOWN_DEF_UNIT 64
#define NODE_HOEDOWN_DEF_MIN_SIZE 0
#define NODE_HOEDOWN_DEF_MAX_SIZE 0
#define NODE_HOEDOWN_DEF_MAX_NESTING 16

#define NODE_HOEDOWN_UNPACK_INT(OBJ, NAME, VAR)                                \
  value = v8u::Int(OBJ->Get(v8u::Symbol(NAME)));                               \
  if (value) VAR = value;

#define NODE_HOEDOWN_SIMPLE_HANDLER(CPP_NAME, V8_NAME, FUNC)                   \
  class CPP_NAME : public node::ObjectWrap {                                   \
  public:                                                                      \
    static V8_SCB(Do) {                                                        \
      CPP_NAME* obj = (CPP_NAME*) info.This()->GetPointerFromInternalField(0); \
      hoedown_buffer* ob = obj->ob;                                            \
      String::Utf8Value input (info[0]);                                       \
                                                                               \
      hoedown_buffer_reset(ob);                                                \
      FUNC(ob, (uint8_t*)*input, input.length());                              \
      return String::New((char*)ob->data, ob->size);                           \
    }                                                                          \
                                                                               \
    V8_CTOR() {                                                                \
      size_t unit = NODE_HOEDOWN_DEF_UNIT;                                     \
      size_t minSize = NODE_HOEDOWN_DEF_MIN_SIZE;                              \
      size_t maxSize = NODE_HOEDOWN_DEF_MAX_SIZE;                              \
      if (info[0]->IsObject()) {                                               \
        Local<Object> opts = v8u::Obj(info[0]);                                \
        int value;                                                             \
        NODE_HOEDOWN_UNPACK_INT(opts, "unit", unit);                           \
        NODE_HOEDOWN_UNPACK_INT(opts, "minimumSize", minSize);                 \
        NODE_HOEDOWN_UNPACK_INT(opts, "maximumSize", maxSize);                 \
      }                                                                        \
      V8_WRAP(new CPP_NAME(unit, minSize, maxSize));                           \
    } V8_CTOR_END()                                                            \
                                                                               \
    NODE_TYPE(CPP_NAME, V8_NAME) {                                             \
      V8_DEF_CB("do", Do);                                                     \
    } NODE_TYPE_END()                                                          \
                                                                               \
    hoedown_buffer* ob;                                                        \
    CPP_NAME(size_t unit, size_t minSize, size_t maxSize) {                    \
      ob = hoedown_buffer_new(unit);                                           \
      hoedown_buffer_grow(ob, minSize);                                        \
    }                                                                          \
    ~CPP_NAME() {                                                              \
      hoedown_buffer_free(ob);                                                 \
    }                                                                          \
  }; V8_POST_TYPE(CPP_NAME)

// Flexible flag parsing
inline unsigned int parseFlags(Handle<Value> value) {
  if (value->IsArray()) {
    Handle<Array> arr = v8u::Arr(value);
    int length = arr->Length();
    unsigned int flags = 0;

    for (int i=0; i<length; i++)
      flags |= v8u::Int(arr->Get(i));
    return flags;
  }
  return v8u::Int(value);
}

#endif /* NODE_HOEDOWN_COMMON_HPP */
