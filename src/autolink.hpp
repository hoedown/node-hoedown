#ifndef NODE_HOEDOWN_AUTOLINK_HPP
#define NODE_HOEDOWN_AUTOLINK_HPP

#include "common.hpp"

#include "hoedown/autolink.h"

namespace Autolink {
  V8_SCB(IsSafe) {
    String::Utf8Value link (info[0]);
    return v8u::Bool(hoedown_autolink_is_safe((uint8_t*)*link, link.length()));
  }
  
  struct AutolinkData {
    size_t (*func)(size_t* rewind_p, hoedown_buffer* link, uint8_t* data, size_t offset, size_t size, unsigned int flags);
    unsigned int flags;
  };
  
  V8_SCB(Autolink) {
    //TODO
  }
  
  V8_SCB(AutolinkCall) {
    //TODO
  }
  
  //TODO: destructor

  NODE_DEF(init) {
    V8_HANDLE_SCOPE(scope);
    
    // autolink function (and namespace)
    target = v8u::SetMethod(target, &Autolink, "autolink");
    
    // flags: Flags
    Local<Object> flags = v8u::Obj();
    flags->Set(v8u::Symbol("SHORT_DOMAINS"), v8u::Int(HOEDOWN_AUTOLINK_SHORT_DOMAINS));
    target->Set(v8u::Symbol("Flags"), flags);

    // other methods
    v8u::SetMethod(target, &IsSafe, "isSafe");
  }
}

#endif /* NODE_HOEDOWN_AUTOLINK_HPP */
