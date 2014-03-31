#ifndef NODE_HOEDOWN_ESCAPE_HPP
#define NODE_HOEDOWN_ESCAPE_HPP

#include "common.hpp"

#include "hoedown/escape.h"

namespace Escape {
  // TODO escapeHTML
  NODE_HOEDOWN_SIMPLE_HANDLER(EscapeHref, "escapeHref", hoedown_escape_href)

  NODE_DEF(init) {
    V8_HANDLE_SCOPE(scope);
    
    //EscapeHTML::init(target);
    EscapeHref::init(target);
  }
}

#endif /* NODE_HOEDOWN_ESCAPE_HPP */
