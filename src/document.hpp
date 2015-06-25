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
    static Persistent<FunctionTemplate> constructor;
    static void init(Handle<Object> target) {
      Local<String> name = NanNew("Hoedown");
      Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
      NanAssignPersistent(constructor, tpl);
      tpl->SetClassName(name);
      tpl->InstanceTemplate()->SetInternalFieldCount(1);

      NODE_SET_PROTOTYPE_METHOD(tpl, "do", Do);

      target->Set(name, tpl->GetFunction());
    }

    static NAN_METHOD(Do) {
      NanScope();
      Hoedown* obj = Unwrap<Hoedown>(args.Holder());
      hoedown_buffer* ob = obj->ob;
      NanUtf8String input (args[0]);

      if (ob->asize > obj->maxSize) {
        free(ob->data);
        ob->data = (uint8_t*) hoedown_malloc(obj->minSize);
        ob->asize = obj->minSize;
      }
      ob->size = 0;

      if (obj->inline_)
        hoedown_document_render_inline(obj->doc, ob, (uint8_t*)*input, input.length());
      else
        hoedown_document_render(obj->doc, ob, (uint8_t*)*input, input.length());

      NanReturnValue(NanNew<String>((char*)ob->data, ob->size));
    }

    static NAN_METHOD(New) {
      NODE_HOEDOWN_CONSTRUCTOR_START() {
        size_t unit = NODE_HOEDOWN_DEF_UNIT;
        size_t minSize = NODE_HOEDOWN_DEF_MIN_SIZE;
        size_t maxSize = NODE_HOEDOWN_DEF_MAX_SIZE;
        bool inline_ = false;
        hoedown_extensions extensions = (hoedown_extensions)0;
        size_t maxNesting = NODE_HOEDOWN_DEF_MAX_NESTING;
        RendererType type = RENDERER_HTML;
        int flags = 0;
        int tocLevel = 0;

        if (args[0]->IsObject()) {
          Local<Object> opts = args[0]->ToObject();
          int value;

          NODE_HOEDOWN_UNPACK_INT(opts, "unit", unit);
          NODE_HOEDOWN_UNPACK_INT(opts, "minimumSize", minSize);
          NODE_HOEDOWN_UNPACK_INT(opts, "maximumSize", maxSize);

          inline_ = opts->Get(NanNew("inline"))->BooleanValue();

          extensions = (hoedown_extensions) parseFlags(opts->Get(NanNew("extensions")));
          NODE_HOEDOWN_UNPACK_INT(opts, "maxNesting", maxNesting);

          Local<Value> rval = opts->Get(NanNew("renderer"));
          if (rval->IsObject()) {
            Local<Object> rndr = rval->ToObject();
            if (rndr->Has(NanNew("type"))) {
              Local<Value> jstype = rndr->Get(NanNew("type"));
              if (jstype == HTML::html) type = RENDERER_HTML;
              else if (jstype == HTML::html_toc) type = RENDERER_HTML_TOC;
              else NanThrowTypeError("Unknown renderer type found.");
            }
            flags = parseFlags(rndr->Get(NanNew("flags")));
            tocLevel = rndr->Get(NanNew("tocLevel"))->IntegerValue();
          }
        }

        (new Hoedown(unit, minSize, maxSize, inline_, extensions, maxNesting, type, flags, tocLevel))->Wrap(args.This());
        NanReturnThis();
      }
    }

    hoedown_buffer* ob;
    hoedown_document* doc;
    hoedown_renderer* rndr;
    void (*rndr_free)(hoedown_renderer* rndr);
    size_t minSize, maxSize;
    bool inline_;

    Hoedown(size_t unit, size_t minSize, size_t maxSize,
            bool inline_, hoedown_extensions extensions, size_t maxNesting,
            RendererType type,
            int flags, int tocLevel):
          minSize(minSize), maxSize(maxSize), inline_(inline_) {
      if (unit < 1) unit = 1;
      if (maxSize < minSize) maxSize = minSize;

      ob = hoedown_buffer_new(unit);
      hoedown_buffer_grow(ob, minSize);

      switch (type) {
        case RENDERER_HTML:
          rndr = hoedown_html_renderer_new((hoedown_html_flags)flags, tocLevel);
          rndr_free = &hoedown_html_renderer_free;
          break;
        case RENDERER_HTML_TOC:
          rndr = hoedown_html_toc_renderer_new(tocLevel);
          rndr_free = &hoedown_html_renderer_free;
          break;
      };

      doc = hoedown_document_new(rndr, extensions, maxNesting);
    }
    ~Hoedown() {
      hoedown_buffer_free(ob);
      rndr_free(rndr);
      hoedown_document_free(doc);
    }
  };
  Persistent<FunctionTemplate> Hoedown::constructor;

  void init(Handle<Object> target) {
    // flags: Extension
    Local<Object> exts = NanNew<Object>();
    exts->Set(NanNew("TABLES"), NanNew(HOEDOWN_EXT_TABLES));
    exts->Set(NanNew("FENCED_CODE"), NanNew(HOEDOWN_EXT_FENCED_CODE));
    exts->Set(NanNew("FOOTNOTES"), NanNew(HOEDOWN_EXT_FOOTNOTES));
    exts->Set(NanNew("AUTOLINK"), NanNew(HOEDOWN_EXT_AUTOLINK));
    exts->Set(NanNew("STRIKETHROUGH"), NanNew(HOEDOWN_EXT_STRIKETHROUGH));
    exts->Set(NanNew("UNDERLINE"), NanNew(HOEDOWN_EXT_UNDERLINE));
    exts->Set(NanNew("HIGHLIGHT"), NanNew(HOEDOWN_EXT_HIGHLIGHT));
    exts->Set(NanNew("QUOTE"), NanNew(HOEDOWN_EXT_QUOTE));
    exts->Set(NanNew("SUPERSCRIPT"), NanNew(HOEDOWN_EXT_SUPERSCRIPT));
    exts->Set(NanNew("MATH"), NanNew(HOEDOWN_EXT_MATH));
    exts->Set(NanNew("NO_INTRA_EMPHASIS"), NanNew(HOEDOWN_EXT_NO_INTRA_EMPHASIS));
    exts->Set(NanNew("SPACE_HEADERS"), NanNew(HOEDOWN_EXT_SPACE_HEADERS));
    exts->Set(NanNew("MATH_EXPLICIT"), NanNew(HOEDOWN_EXT_MATH_EXPLICIT));
    exts->Set(NanNew("DISABLE_INDENTED_CODE"), NanNew(HOEDOWN_EXT_DISABLE_INDENTED_CODE));
    target->Set(NanNew("Extensions"), exts);

    // extension categories
    target->Set(NanNew("EXT_BLOCK"), NanNew(HOEDOWN_EXT_BLOCK));
    target->Set(NanNew("EXT_SPAN"), NanNew(HOEDOWN_EXT_SPAN));
    target->Set(NanNew("EXT_FLAGS"), NanNew(HOEDOWN_EXT_FLAGS));
    target->Set(NanNew("EXT_NEGATIVE"), NanNew(HOEDOWN_EXT_NEGATIVE));

    // flags: ListFlags
    Local<Object> listflags = NanNew<Object>();
    listflags->Set(NanNew("LIST_ORDERED"), NanNew(HOEDOWN_LIST_ORDERED));
    listflags->Set(NanNew("LI_BLOCK"), NanNew(HOEDOWN_LI_BLOCK));
    target->Set(NanNew("ListFlags"), listflags);

    // flags: TableFlags
    Local<Object> tableflags = NanNew<Object>();
    tableflags->Set(NanNew("ALIGN_LEFT"), NanNew(HOEDOWN_TABLE_ALIGN_LEFT));
    tableflags->Set(NanNew("ALIGN_RIGHT"), NanNew(HOEDOWN_TABLE_ALIGN_RIGHT));
    tableflags->Set(NanNew("ALIGN_CENTER"), NanNew(HOEDOWN_TABLE_ALIGN_CENTER));
    tableflags->Set(NanNew("ALIGNMASK"), NanNew(HOEDOWN_TABLE_ALIGNMASK));
    tableflags->Set(NanNew("HEADER"), NanNew(HOEDOWN_TABLE_HEADER));
    target->Set(NanNew("TableFlags"), tableflags);

    // enum: AutolinkType
    Local<Object> autolink = NanNew<Object>();
    autolink->Set(NanNew("NONE"), NanNew(HOEDOWN_AUTOLINK_NONE));
    autolink->Set(NanNew("NORMAL"), NanNew(HOEDOWN_AUTOLINK_NORMAL));
    autolink->Set(NanNew("EMAIL"), NanNew(HOEDOWN_AUTOLINK_EMAIL));
    target->Set(NanNew("AutolinkType"), autolink);
  }
}

#endif /* NODE_HOEDOWN_DOCUMENT_HPP */
