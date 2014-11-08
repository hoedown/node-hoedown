#ifndef NODE_HOEDOWN_ESCAPE_HPP
#define NODE_HOEDOWN_ESCAPE_HPP

#include "common.hpp"

#include "hoedown/escape.h"

namespace Escape {
  class EscapeHTML : public node::ObjectWrap {
  public:
    static V8_SCB(Do) {
      EscapeHTML* obj = (EscapeHTML*) info.This()->GetPointerFromInternalField(0);
      hoedown_buffer* ob = obj->ob;
      String::Utf8Value input (info[0]);

      if (ob->asize > obj->maxSize) {
        free(ob->data);
        ob->data = (uint8_t*) malloc(obj->minSize);
        ob->asize = obj->minSize;
      }
      ob->size = 0;

      hoedown_escape_html(ob, (uint8_t*)*input, input.length(), obj->secure);
      return String::New((char*)ob->data, ob->size);
    }

    V8_CTOR() {
      size_t unit = NODE_HOEDOWN_DEF_UNIT;
      size_t minSize = NODE_HOEDOWN_DEF_MIN_SIZE;
      size_t maxSize = NODE_HOEDOWN_DEF_MAX_SIZE;
      bool secure = false;

      if (info[0]->IsObject()) {
        Local<Object> opts = v8u::Obj(info[0]);
        int value;

        NODE_HOEDOWN_UNPACK_INT(opts, "unit", unit);
        NODE_HOEDOWN_UNPACK_INT(opts, "minimumSize", minSize);
        NODE_HOEDOWN_UNPACK_INT(opts, "maximumSize", maxSize);

        secure = v8u::Bool(opts->Get(v8u::Symbol("secure")));
      }

      V8_WRAP(new EscapeHTML(unit, minSize, maxSize, secure));
    } V8_CTOR_END()

    NODE_TYPE(EscapeHTML, "escapeHTML") {
      V8_DEF_CB("do", Do);
    } NODE_TYPE_END()

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
  }; V8_POST_TYPE(EscapeHTML);

  NODE_HOEDOWN_SIMPLE_HANDLER(EscapeHref, "escapeHref", hoedown_escape_href)

  NODE_DEF(init) {
    V8_HANDLE_SCOPE(scope);

    EscapeHTML::init(target);
    EscapeHref::init(target);
  }
}

#endif /* NODE_HOEDOWN_ESCAPE_HPP */
