#ifndef NODE_HOEDOWN_COMMON_HPP
#define NODE_HOEDOWN_COMMON_HPP

#include "v8u/v8u.hpp"
using namespace v8;

#define NODE_HOEDOWN_DEF_UNIT 64
#define NODE_HOEDOWN_DEF_SIZE  0

#define NODE_HOEDOWN_UNPACK_INT(OBJ, NAME, VAR)                                \
  if (OBJ->Has(v8u::Symbol(NAME))) VAR = v8u::Int(OBJ->Get(v8u::Symbol(NAME)))

#define NODE_HOEDOWN_SIMPLE_HANDLER(ID, FUNC)                                  \
  V8_SCB(ID) {                                                                 \
    size_t unit = NODE_HOEDOWN_DEF_UNIT;                                       \
    size_t size = NODE_HOEDOWN_DEF_SIZE;                                       \
    if (info[0]->IsObject()) {                                                 \
      Local<Object> opts = v8u::Obj(info[0]);                                  \
      NODE_HOEDOWN_UNPACK_INT(opts, "unit", unit);                             \
      NODE_HOEDOWN_UNPACK_INT(opts, "initialSize", size);                      \
    }                                                                          \
    hoedown_buffer* ob = hoedown_buffer_new(unit);                             \
    hoedown_buffer_grow(ob, size);                                             \
  }                                                                            \
                                                                               \
  V8_SCB(ID##Call) {                                                           \
    String::Utf8Value input (info[0]);                                         \
  }

//TODO: generic deallocator

#endif /* NODE_HOEDOWN_COMMON_HPP */
