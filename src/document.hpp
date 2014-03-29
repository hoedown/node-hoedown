#ifndef NODE_HOEDOWN_DOCUMENT_HPP
#define NODE_HOEDOWN_DOCUMENT_HPP

#include "common.hpp"
#include "html.hpp"

#include "hoedown/document.h"

namespace Document {
  struct HoedownData {
    hoedown_buffer* ob;
    hoedown_buffer* ob2;
    hoedown_document* doc;
    hoedown_renderer* rndr;
    void (*rndr_free)(hoedown_renderer* rndr);
  };

  V8_SCB(Hoedown) {
    if (info[0]->IsObject()) {
      Local<Object> opts = v8u::Obj(info[0]);
    }
  }

  V8_SCB(HoedownCall) {
    //TODO
  }

  NODE_DEF(init) {
    // flags: Extension
    Local<Object> exts = v8u::Obj();
    exts->Set(v8u::Symbol("TABLES"), v8u::Int(HOEDOWN_EXT_TABLES));
    exts->Set(v8u::Symbol("FENCED_CODE"), v8u::Int(HOEDOWN_EXT_FENCED_CODE));
    exts->Set(v8u::Symbol("FOOTNOTES"), v8u::Int(HOEDOWN_EXT_FOOTNOTES));
    exts->Set(v8u::Symbol("AUTOLINK"), v8u::Int(HOEDOWN_EXT_AUTOLINK));
    exts->Set(v8u::Symbol("STRIKETHROUGH"), v8u::Int(HOEDOWN_EXT_STRIKETHROUGH));
    exts->Set(v8u::Symbol("UNDERLINE"), v8u::Int(HOEDOWN_EXT_UNDERLINE));
    exts->Set(v8u::Symbol("HIGHLIGHT"), v8u::Int(HOEDOWN_EXT_HIGHLIGHT));
    exts->Set(v8u::Symbol("QUOTE"), v8u::Int(HOEDOWN_EXT_QUOTE));
    exts->Set(v8u::Symbol("SUPERSCRIPT"), v8u::Int(HOEDOWN_EXT_SUPERSCRIPT));
    exts->Set(v8u::Symbol("LAX_SPACING"), v8u::Int(HOEDOWN_EXT_LAX_SPACING));
    exts->Set(v8u::Symbol("NO_INTRA_EMPHASIS"), v8u::Int(HOEDOWN_EXT_NO_INTRA_EMPHASIS));
    exts->Set(v8u::Symbol("SPACE_HEADERS"), v8u::Int(HOEDOWN_EXT_SPACE_HEADERS));
    exts->Set(v8u::Symbol("DISABLE_INDENTED_CODE"), v8u::Int(HOEDOWN_EXT_DISABLE_INDENTED_CODE));
    target->Set(v8u::Symbol("Extension"), exts);
  }
}

#endif /* NODE_HOEDOWN_DOCUMENT_HPP */
