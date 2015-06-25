#ifndef NODE_HOEDOWN_ESCAPE_HPP
#define NODE_HOEDOWN_ESCAPE_HPP

#include "common.hpp"

#include "hoedown/escape.h"

namespace Escape {
  class EscapeHTML : public node::ObjectWrap {
  public:
    static Persistent<FunctionTemplate> constructor;
    static void init(Handle<Object> target) {
      Local<String> name = NanNew("escapeHTML");
      Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
      NanAssignPersistent(constructor, tpl);
      tpl->SetClassName(name);
      tpl->InstanceTemplate()->SetInternalFieldCount(1);

      NODE_SET_PROTOTYPE_METHOD(tpl, "do", Do);

      target->Set(name, tpl->GetFunction());
    }

    static NAN_METHOD(Do) {
      NanScope();
      EscapeHTML* obj = Unwrap<EscapeHTML>(args.Holder());
      hoedown_buffer* ob = obj->ob;
      NanUtf8String input (args[0]);

      if (ob->asize > obj->maxSize) {
        free(ob->data);
        ob->data = (uint8_t*) malloc(obj->minSize);
        ob->asize = obj->minSize;
      }
      ob->size = 0;

      hoedown_escape_html(ob, (uint8_t*)*input, input.length(), obj->secure);
      NanReturnValue(NanNew<String>((char*)ob->data, ob->size));
    }

    static NAN_METHOD(New) {
      NODE_HOEDOWN_CONSTRUCTOR_START() {
        size_t unit = NODE_HOEDOWN_DEF_UNIT;
        size_t minSize = NODE_HOEDOWN_DEF_MIN_SIZE;
        size_t maxSize = NODE_HOEDOWN_DEF_MAX_SIZE;
        bool secure = false;

        if (args[0]->IsObject()) {
          Local<Object> opts = args[0]->ToObject();
          int value;

          NODE_HOEDOWN_UNPACK_INT(opts, "unit", unit);
          NODE_HOEDOWN_UNPACK_INT(opts, "minimumSize", minSize);
          NODE_HOEDOWN_UNPACK_INT(opts, "maximumSize", maxSize);

          secure = opts->Get(NanNew("secure"))->BooleanValue();
        }

        (new EscapeHTML(unit, minSize, maxSize, secure))->Wrap(args.This());
        NanReturnThis();
      }
    }

    hoedown_buffer* ob;
    bool secure;
    size_t minSize, maxSize;

    EscapeHTML(size_t unit, size_t minSiz, size_t maxSiz, bool secure):
        secure(secure), minSize(minSiz), maxSize(maxSiz) {
      if (unit < 1) unit = 1;
      if (maxSize < minSize) maxSize = minSize;

      ob = hoedown_buffer_new(unit);
      hoedown_buffer_grow(ob, minSize);
    }
    ~EscapeHTML() {
      hoedown_buffer_free(ob);
    }
  };
  Persistent<FunctionTemplate> EscapeHTML::constructor;

  NODE_HOEDOWN_SIMPLE_HANDLER(EscapeHref, "escapeHref", hoedown_escape_href)

  void init(Handle<Object> target) {
    EscapeHTML::init(target);
    EscapeHref::init(target);
  }
}

#endif /* NODE_HOEDOWN_ESCAPE_HPP */
