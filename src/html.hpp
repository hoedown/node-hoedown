#ifndef NODE_HOEDOWN_HTML_HPP
#define NODE_HOEDOWN_HTML_HPP

#include "common.hpp"

#include "hoedown/html.h"

namespace HTML {
  Persistent<Object> html_;
  Persistent<Object> htmltoc_;

  NODE_HOEDOWN_SIMPLE_HANDLER(Smartypants, hoedown_html_smartypants)

  NODE_DEF(init) {
    // HTML renderer
    Local<Object> html = v8u::Obj();
    target->Set(v8u::Symbol("HTML"), html);
    html_ = v8u::Persist(html);

    // flags
    html->Set(v8u::Symbol("SKIP_HTML"), v8u::Int(HOEDOWN_HTML_SKIP_HTML));
    html->Set(v8u::Symbol("SKIP_STYLE"), v8u::Int(HOEDOWN_HTML_SKIP_STYLE));
    html->Set(v8u::Symbol("SKIP_IMAGES"), v8u::Int(HOEDOWN_HTML_SKIP_IMAGES));
    html->Set(v8u::Symbol("SKIP_LINKS"), v8u::Int(HOEDOWN_HTML_SKIP_LINKS));
    html->Set(v8u::Symbol("EXPAND_TABS"), v8u::Int(HOEDOWN_HTML_EXPAND_TABS));
    html->Set(v8u::Symbol("SAFELINK"), v8u::Int(HOEDOWN_HTML_SAFELINK));
    html->Set(v8u::Symbol("TOC"), v8u::Int(HOEDOWN_HTML_TOC));
    html->Set(v8u::Symbol("HARD_WRAP"), v8u::Int(HOEDOWN_HTML_HARD_WRAP));
    html->Set(v8u::Symbol("USE_XHTML"), v8u::Int(HOEDOWN_HTML_USE_XHTML));
    html->Set(v8u::Symbol("ESCAPE"), v8u::Int(HOEDOWN_HTML_ESCAPE));

    // enum: Tag
    Local<Object> tag = v8u::Obj();
    tag->Set(v8u::Symbol("NONE"), v8u::Int(HOEDOWN_HTML_TAG_NONE));
    tag->Set(v8u::Symbol("OPEN"), v8u::Int(HOEDOWN_HTML_TAG_OPEN));
    tag->Set(v8u::Symbol("CLOSE"), v8u::Int(HOEDOWN_HTML_TAG_CLOSE));
    html->Set(v8u::Symbol("Tag"), tag);

    // HTML TOC renderer
    Local<Object> htmltoc = v8u::Obj();
    target->Set(v8u::Symbol("HTMLTOC"), htmltoc);
    htmltoc_ = v8u::Persist(htmltoc);

    // SmartyPants
    Local<Function> smartypants = v8u::Func(Smartypants, "smartypantsHTML");
    target->Set(v8u::Symbol("smartypantsHTML"), smartypants);
  }
}

#endif /* NODE_HOEDOWN_HTML_HPP */
