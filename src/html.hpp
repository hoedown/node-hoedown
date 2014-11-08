#ifndef NODE_HOEDOWN_HTML_HPP
#define NODE_HOEDOWN_HTML_HPP

#include "common.hpp"

#include "hoedown/html.h"

namespace HTML {
  Persistent<Object> html;
  Persistent<Object> html_toc;

  NODE_HOEDOWN_SIMPLE_HANDLER(Smartypants, "smartypants", hoedown_html_smartypants)
  
  V8_SCB(IsTag) {
    String::Utf8Value html (info[0]);
    String::Utf8Value tagname (info[1]);
    return v8u::Int(hoedown_html_is_tag((uint8_t*)*html, html.length(), *tagname));
  }

  NODE_DEF(init) {
    V8_HANDLE_SCOPE(scope);
    
    // HTML renderer (and namespace)
    html = v8u::Persist(v8u::Obj());
    target->Set(v8u::Symbol("HTML"), html);
    target = html;

    // flags: Flags
    Local<Object> flags = v8u::Obj();
    flags->Set(v8u::Symbol("SKIP_HTML"), v8u::Int(HOEDOWN_HTML_SKIP_HTML));
    flags->Set(v8u::Symbol("ESCAPE"), v8u::Int(HOEDOWN_HTML_ESCAPE));
    flags->Set(v8u::Symbol("HARD_WRAP"), v8u::Int(HOEDOWN_HTML_HARD_WRAP));
    flags->Set(v8u::Symbol("USE_XHTML"), v8u::Int(HOEDOWN_HTML_USE_XHTML));
    target->Set(v8u::Symbol("HTML"), flags);

    // enum: TagType
    Local<Object> tag = v8u::Obj();
    tag->Set(v8u::Symbol("NONE"), v8u::Int(HOEDOWN_HTML_TAG_NONE));
    tag->Set(v8u::Symbol("OPEN"), v8u::Int(HOEDOWN_HTML_TAG_OPEN));
    tag->Set(v8u::Symbol("CLOSE"), v8u::Int(HOEDOWN_HTML_TAG_CLOSE));
    target->Set(v8u::Symbol("TagType"), tag);

    // HTML TOC renderer
    html_toc = v8u::Persist(v8u::Obj());
    target->Set(v8u::Symbol("TOC"), html_toc);

    // other methods
    Smartypants::init(target);
    v8u::SetMethod(target, &IsTag, "isTag");
  }
}

#endif /* NODE_HOEDOWN_HTML_HPP */
