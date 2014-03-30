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
  
  //TODO: destructor

  NODE_DEF(init) {
    V8_HANDLE_SCOPE(scope);
    
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
    target->Set(v8u::Symbol("Extensions"), exts);

    // extension categories
    target->Set(v8u::Symbol("EXT_BLOCK"), v8u::Int(HOEDOWN_EXT_BLOCK));
    target->Set(v8u::Symbol("EXT_SPAN"), v8u::Int(HOEDOWN_EXT_SPAN));
    target->Set(v8u::Symbol("EXT_FLAGS"), v8u::Int(HOEDOWN_EXT_FLAGS));
    target->Set(v8u::Symbol("EXT_NEGATIVE"), v8u::Int(HOEDOWN_EXT_NEGATIVE));

    // flags: ListFlags
    Local<Object> listflags = v8u::Obj();
    listflags->Set(v8u::Symbol("LIST_ORDERED"), v8u::Int(HOEDOWN_LIST_ORDERED));
    listflags->Set(v8u::Symbol("LI_BLOCK"), v8u::Int(HOEDOWN_LI_BLOCK));
    target->Set(v8u::Symbol("ListFlags"), listflags);

    // flags: TableFlags
    Local<Object> tableflags = v8u::Obj();
    tableflags->Set(v8u::Symbol("ALIGN_LEFT"), v8u::Int(HOEDOWN_TABLE_ALIGN_LEFT));
    tableflags->Set(v8u::Symbol("ALIGN_RIGHT"), v8u::Int(HOEDOWN_TABLE_ALIGN_RIGHT));
    tableflags->Set(v8u::Symbol("ALIGN_CENTER"), v8u::Int(HOEDOWN_TABLE_ALIGN_CENTER));
    tableflags->Set(v8u::Symbol("ALIGNMASK"), v8u::Int(HOEDOWN_TABLE_ALIGNMASK));
    tableflags->Set(v8u::Symbol("HEADER"), v8u::Int(HOEDOWN_TABLE_HEADER));
    target->Set(v8u::Symbol("TableFlags"), tableflags);
    
    // enum: AutolinkType
    Local<Object> autolink = v8u::Obj();
    autolink->Set(v8u::Symbol("NONE"), v8u::Int(HOEDOWN_AUTOLINK_NONE));
    autolink->Set(v8u::Symbol("NORMAL"), v8u::Int(HOEDOWN_AUTOLINK_NORMAL));
    autolink->Set(v8u::Symbol("EMAIL"), v8u::Int(HOEDOWN_AUTOLINK_EMAIL));
    target->Set(v8u::Symbol("AutolinkType"), autolink);
  }
}

#endif /* NODE_HOEDOWN_DOCUMENT_HPP */
