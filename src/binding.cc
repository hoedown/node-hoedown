#include "common.hpp"
#include "autolink.hpp"
#include "document.hpp"
#include "escape.hpp"
#include "html.hpp"

#include "v8u/version.hpp"
#include "hoedown/version.h"

void exposeHoedownVersion(Handle<Object> target) {
  int major, minor, revision;
  hoedown_version(&major, &minor, &revision);
  Local<Object> version = (new v8u::Version(major, minor, revision))->Wrapped();
  target->Set(v8u::Symbol("version"), version);
}


NODE_DEF_MAIN() {
  // create module function
  Document::Hoedown::init(target);
  target = Document::Hoedown::templ_->GetFunction();

  // all the other things
  Autolink::init(target);
  Document::init(target);
  Escape::init(target);
  HTML::init(target);

  // version
  v8u::Version::init(target);
  exposeHoedownVersion(target);
} NODE_DEF_MAIN_END(hoedown)
