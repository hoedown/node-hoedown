#ifndef NODE_HOEDOWN_COMMON_HPP
#define NODE_HOEDOWN_COMMON_HPP

#include <node.h>
#include <nan.h>
using namespace v8;

#define NODE_HOEDOWN_DEF_UNIT 64
#define NODE_HOEDOWN_DEF_MIN_SIZE 0
#define NODE_HOEDOWN_DEF_MAX_SIZE 0
#define NODE_HOEDOWN_DEF_MAX_NESTING 16

#define NODE_HOEDOWN_CONSTRUCTOR_START()                                       \
  NanScope();                                                                  \
  if (!args.IsConstructCall()) {                                               \
    size_t length = args.Length();                                             \
    Local<Value> nargs [length];                                               \
    for (size_t i = 0; i < length; i++)                                        \
      nargs[i] = args[i];                                                      \
    NanReturnValue(NanNew(constructor)->GetFunction()->NewInstance(length, nargs));\
  } else

#define NODE_HOEDOWN_UNPACK_INT(OBJ, NAME, VAR)                                \
  value = (OBJ)->Get(NanNew(NAME))->IntegerValue();                            \
  if (value) VAR = value;

#define NODE_HOEDOWN_SIMPLE_HANDLER(CPP_NAME, V8_NAME, FUNC)                   \
  class CPP_NAME : public node::ObjectWrap {                                   \
  public:                                                                      \
    static Persistent<FunctionTemplate> constructor;                           \
    static void init(Handle<Object> target) {                                  \
      Local<String> name = NanNew(V8_NAME);                                    \
      Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);             \
      NanAssignPersistent(constructor, tpl);                                   \
      tpl->SetClassName(name);                                                 \
      tpl->InstanceTemplate()->SetInternalFieldCount(1);                       \
                                                                               \
      NODE_SET_PROTOTYPE_METHOD(tpl, "do", Do);                                \
                                                                               \
      target->Set(name, tpl->GetFunction());                                   \
    }                                                                          \
                                                                               \
    static NAN_METHOD(Do) {                                                    \
      NanScope();                                                              \
      CPP_NAME* obj = Unwrap<CPP_NAME>(args.Holder());                         \
      hoedown_buffer* ob = obj->ob;                                            \
      NanUtf8String input (args[0]);                                           \
                                                                               \
      if (ob->asize > obj->maxSize) {                                          \
        free(ob->data);                                                        \
        ob->data = (uint8_t*) malloc(obj->minSize);                            \
        ob->asize = obj->minSize;                                              \
      }                                                                        \
      ob->size = 0;                                                            \
                                                                               \
      FUNC(ob, (uint8_t*)*input, input.length());                              \
      NanReturnValue(NanNew<String>((char*)ob->data, ob->size));               \
    }                                                                          \
                                                                               \
    static NAN_METHOD(New) {                                                   \
      NODE_HOEDOWN_CONSTRUCTOR_START() {                                       \
        size_t unit = NODE_HOEDOWN_DEF_UNIT;                                   \
        size_t minSize = NODE_HOEDOWN_DEF_MIN_SIZE;                            \
        size_t maxSize = NODE_HOEDOWN_DEF_MAX_SIZE;                            \
        if (args[0]->IsObject()) {                                             \
          Handle<Object> opts = args[0]->ToObject();                           \
          int value;                                                           \
          NODE_HOEDOWN_UNPACK_INT(opts, "unit", unit);                         \
          NODE_HOEDOWN_UNPACK_INT(opts, "minimumSize", minSize);               \
          NODE_HOEDOWN_UNPACK_INT(opts, "maximumSize", maxSize);               \
        }                                                                      \
        (new CPP_NAME(unit, minSize, maxSize))->Wrap(args.This());             \
        NanReturnThis();                                                       \
      }                                                                        \
    }                                                                          \
                                                                               \
    hoedown_buffer* ob;                                                        \
    size_t minSize, maxSize;                                                   \
    CPP_NAME(size_t unit, size_t minSiz, size_t maxSiz):                       \
        minSize(minSiz), maxSize(maxSiz) {                                     \
      if (unit < 1) unit = 1;                                                  \
      if (maxSize < minSize) maxSize = minSize;                                \
                                                                               \
      ob = hoedown_buffer_new(unit);                                           \
      hoedown_buffer_grow(ob, minSize);                                        \
    }                                                                          \
    ~CPP_NAME() {                                                              \
      hoedown_buffer_free(ob);                                                 \
    }                                                                          \
  };                                                                           \
  Persistent<FunctionTemplate> CPP_NAME::constructor;

// Flexible flag parsing
inline unsigned int parseFlags(Handle<Value> value) {
  if (value->IsArray()) {
    Handle<Array> arr = Handle<Array>::Cast(value);
    int length = arr->Length();
    unsigned int flags = 0;

    for (int i=0; i<length; i++)
      flags |= arr->Get(i)->IntegerValue();
    return flags;
  }

  return value->IntegerValue();
}

#endif /* NODE_HOEDOWN_COMMON_HPP */


//TODO: docs (mention exception), make all parts safe, commit benchmark & add to travis tests
//upgrade hoedown
