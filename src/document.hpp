#ifndef NODE_HOEDOWN_DOCUMENT_HPP
#define NODE_HOEDOWN_DOCUMENT_HPP

#include "common.hpp"
#include "html.hpp"

#include "hoedown/document.h"

namespace Document {
  enum RendererType {
    RENDERER_HTML,
    RENDERER_HTML_TOC,
  };
  
  class Hoedown : public node::ObjectWrap {
  public:
    static V8_SCB(Do) {
      Hoedown* obj = (Hoedown*) info.This()->GetPointerFromInternalField(0);
      hoedown_buffer* ob = obj->ob;
      String::Utf8Value input (info[0]);

      if (ob->asize > obj->maxSize) {
        free(ob->data);
        ob->data = (uint8_t*) malloc(obj->minSize);
        if (!ob->data) V8_STHROW(v8u::Err("No memory."));
        ob->asize = obj->minSize;
      }
      ob->size = 0;

      hoedown_document_render(obj->doc, ob, (uint8_t*)*input, input.length());
      if (obj->sp) {
        ob = obj->sp;

        if (ob->asize > obj->maxSize) {
          free(ob->data);
          ob->data = (uint8_t*) malloc(obj->minSize);
          if (!ob->data) V8_STHROW(v8u::Err("No memory."));
          ob->asize = obj->minSize;
        }
        ob->size = 0;

        hoedown_html_smartypants(ob, obj->ob->data, obj->ob->size);
      }
      return String::New((char*)ob->data, ob->size);
    }

    V8_CTOR() {
      size_t unit = NODE_HOEDOWN_DEF_UNIT;
      size_t minSize = NODE_HOEDOWN_DEF_MIN_SIZE;
      size_t maxSize = NODE_HOEDOWN_DEF_MAX_SIZE;
      int extensions = 0;
      size_t maxNesting = NODE_HOEDOWN_DEF_MAX_NESTING;
      RendererType type = RENDERER_HTML;
      int flags = 0;
      bool smartypants = false;
      int tocLevel = 0;

      if (info[0]->IsObject()) {
        Local<Object> opts = v8u::Obj(info[0]);
        int value;

        NODE_HOEDOWN_UNPACK_INT(opts, "unit", unit);
        NODE_HOEDOWN_UNPACK_INT(opts, "minimumSize", minSize);
        NODE_HOEDOWN_UNPACK_INT(opts, "maximumSize", maxSize);

        extensions = parseFlags(opts->Get(v8u::Symbol("extensions")));
        NODE_HOEDOWN_UNPACK_INT(opts, "maxNesting", maxNesting);

        Local<Value> rval = opts->Get(v8u::Symbol("renderer"));
        if (rval->IsObject()) {
          Local<Object> rndr = v8u::Obj(rval);
          if (rndr->Has(v8u::Symbol("type"))) {
            Local<Value> jstype = rndr->Get(v8u::Symbol("type"));
            if (jstype == HTML::html) type = RENDERER_HTML;
            else if (jstype == HTML::html_toc) type = RENDERER_HTML_TOC;
            else V8_THROW(v8u::TypeErr("Unknown renderer type found."));
          }
          flags = parseFlags(rndr->Get(v8u::Symbol("flags")));
          smartypants = v8u::Bool(rndr->Get(v8u::Symbol("smartypants")));
          tocLevel = v8u::Int(rndr->Get(v8u::Symbol("tocLevel")));
        }
      }

      V8_WRAP(new Hoedown(unit, minSize, maxSize, extensions, maxNesting, type, flags, smartypants, tocLevel));
    } V8_CTOR_END()

    NODE_TYPE(Hoedown, "hoedown") {
      V8_DEF_CB("do", Do);
    } NODE_TYPE_END()

    hoedown_buffer* ob;
    hoedown_buffer* sp;
    hoedown_document* doc;
    hoedown_renderer* rndr;
    void (*rndr_free)(hoedown_renderer* rndr);
    size_t minSize, maxSize;

    Hoedown(size_t unit, size_t minSiz, size_t maxSiz,
            int extensions, size_t maxNesting,
            RendererType type,
            int flags, bool smartypants, int tocLevel):
          minSize(minSiz), maxSize(maxSiz) {
      if (unit < 1) unit = 1;
      if (maxSize < minSize) maxSize = minSize;

      ob = hoedown_buffer_new(unit);
      if (!ob || hoedown_buffer_grow(ob, minSize) != HOEDOWN_BUF_OK)
        V8_THROW(v8u::Err("No memory."));

      switch (type) {
        case RENDERER_HTML:
          rndr = hoedown_html_renderer_new(flags, tocLevel);
          rndr_free = &hoedown_html_renderer_free;
          break;
        case RENDERER_HTML_TOC:
          rndr = hoedown_html_toc_renderer_new(tocLevel);
          rndr_free = &hoedown_html_renderer_free;
          break;
      };
      if (!rndr)
        V8_THROW(v8u::Err("No memory."));

      doc = hoedown_document_new(rndr, extensions, maxNesting);
      if (!doc)
        V8_THROW(v8u::Err("No memory."));

      if (smartypants && (type == RENDERER_HTML || type == RENDERER_HTML_TOC)) {
        sp = hoedown_buffer_new(unit);
        if (!sp || hoedown_buffer_grow(sp, minSize) != HOEDOWN_BUF_OK)
          V8_THROW(v8u::Err("No memory."));
      } else sp = NULL;
    }
    ~Hoedown() {
      if (!ob) return;
      hoedown_buffer_free(ob);
      if (!rndr) return;
      rndr_free(rndr);
      if (!doc) return;
      hoedown_document_free(doc);
      if (!sp) return;
      hoedown_buffer_free(sp);
    }
  }; V8_POST_TYPE(Hoedown);

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
