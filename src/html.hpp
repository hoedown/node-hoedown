#ifndef NODE_HOEDOWN_HTML_HPP
#define NODE_HOEDOWN_HTML_HPP

#include "common.hpp"

#include "hoedown/html.h"

namespace HTML {
  Persistent<Object> html;
  Persistent<Object> html_toc;

  NODE_HOEDOWN_SIMPLE_HANDLER(Smartypants, "smartypants", hoedown_html_smartypants)

  NAN_METHOD(IsTag) {
    NanScope();
    NanUtf8String html (args[0]);
    NanUtf8String tagname (args[1]);
    NanReturnValue(NanNew(hoedown_html_is_tag((uint8_t*)*html, html.length(), *tagname)));
  }

  void init(Handle<Object> target) {
    // HTML renderer (and namespace)
    Local<Object> html = NanNew<Object>();
    NanAssignPersistent(HTML::html, html);
    target->Set(NanNew("HTML"), html);
    target = html;

    // flags: Flags
    Local<Object> flags = NanNew<Object>();
    flags->Set(NanNew("SKIP_HTML"), NanNew(HOEDOWN_HTML_SKIP_HTML));
    flags->Set(NanNew("ESCAPE"), NanNew(HOEDOWN_HTML_ESCAPE));
    flags->Set(NanNew("HARD_WRAP"), NanNew(HOEDOWN_HTML_HARD_WRAP));
    flags->Set(NanNew("USE_XHTML"), NanNew(HOEDOWN_HTML_USE_XHTML));
    target->Set(NanNew("Flags"), flags);

    // enum: TagType
    Local<Object> tag = NanNew<Object>();
    tag->Set(NanNew("NONE"), NanNew(HOEDOWN_HTML_TAG_NONE));
    tag->Set(NanNew("OPEN"), NanNew(HOEDOWN_HTML_TAG_OPEN));
    tag->Set(NanNew("CLOSE"), NanNew(HOEDOWN_HTML_TAG_CLOSE));
    target->Set(NanNew("TagType"), tag);

    // HTML TOC renderer
    Local<Object> html_toc = NanNew<Object>();
    NanAssignPersistent(HTML::html_toc, html_toc);
    target->Set(NanNew("TOC"), html_toc);

    // other methods
    Smartypants::init(target);
    NODE_SET_METHOD(target, "isTag", IsTag);
  }
}

#endif /* NODE_HOEDOWN_HTML_HPP */
