#ifndef NODE_HOEDOWN_AUTOLINK_HPP
#define NODE_HOEDOWN_AUTOLINK_HPP

#include "common.hpp"

#include "hoedown/autolink.h"

namespace Autolink {
  NAN_METHOD(IsSafe) {
    NanUtf8String link (args[0]);
    int result = hoedown_autolink_is_safe((uint8_t*)*link, link.length());
    NanReturnValue(NanNew<Boolean>(result));
  }

  struct AutolinkData {
    size_t (*func)(size_t* rewind_p, hoedown_buffer* link, uint8_t* data, size_t offset, size_t size, unsigned int flags);
    unsigned int flags;
  };

  NAN_METHOD(Autolink) {
    //TODO
    NanReturnUndefined();
  }

  NAN_METHOD(AutolinkCall) {
    //TODO
    NanReturnUndefined();
  }

  //TODO: destructor

  void init(Handle<Object> target) {
    // autolink function (and namespace)
    NODE_SET_METHOD(target, "autolink", Autolink);
    target = target->Get(NanNew("autolink"))->ToObject();

    // flags: Flags
    Local<Object> flags = NanNew<Object>();
    flags->Set(NanNew("SHORT_DOMAINS"), NanNew(HOEDOWN_AUTOLINK_SHORT_DOMAINS));
    target->Set(NanNew("Flags"), flags);

    // other methods
    NODE_SET_METHOD(target, "isSafe", IsSafe);
  }
}

#endif /* NODE_HOEDOWN_AUTOLINK_HPP */
