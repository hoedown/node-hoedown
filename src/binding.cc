#include "common.hpp"
#include "autolink.hpp"
#include "document.hpp"
#include "escape.hpp"
#include "html.hpp"

#include "hoedown/version.h"

namespace Version {
  Persistent<FunctionTemplate> constructor;

  NAN_METHOD(New) {
    NODE_HOEDOWN_CONSTRUCTOR_START() {
      int major = args[0]->IntegerValue();
      int minor = args[1]->IntegerValue();
      int revision = args[2]->IntegerValue();
      args.This()->Set(NanNew("major"), NanNew(major));
      args.This()->Set(NanNew("minor"), NanNew(minor));
      args.This()->Set(NanNew("revision"), NanNew(revision));
      NanReturnThis();
    }
  }

  NAN_METHOD(Inspect) {
    NanScope();
    int major = args.Holder()->Get(NanNew("major"))->IntegerValue();
    int minor = args.Holder()->Get(NanNew("minor"))->IntegerValue();
    int revision = args.Holder()->Get(NanNew("revision"))->IntegerValue();
    hoedown_buffer* result = hoedown_buffer_new(16);
    hoedown_buffer_printf(result, "<Version %d.%d.%d>", major, minor, revision);
    Local<String> str = NanNew<String>((char*)result->data, result->size);
    hoedown_buffer_free(result);
    NanReturnValue(str);
  }

  NAN_METHOD(ToString) {
    NanScope();
    int major = args.Holder()->Get(NanNew("major"))->IntegerValue();
    int minor = args.Holder()->Get(NanNew("minor"))->IntegerValue();
    int revision = args.Holder()->Get(NanNew("revision"))->IntegerValue();
    hoedown_buffer* result = hoedown_buffer_new(16);
    hoedown_buffer_printf(result, "%d.%d.%d", major, minor, revision);
    Local<String> str = NanNew<String>((char*)result->data, result->size);
    hoedown_buffer_free(result);
    NanReturnValue(str);
  }

  void init(Handle<Object> target) {
    Local<String> name = NanNew("Version");
    Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
    NanAssignPersistent(constructor, tpl);
    tpl->SetClassName(name);
    NODE_SET_PROTOTYPE_METHOD(tpl, "inspect", Inspect);
    NODE_SET_PROTOTYPE_METHOD(tpl, "toString", ToString);
    target->Set(name, tpl->GetFunction());
  }
}

void exposeHoedownVersion(Handle<Object> target) {
  int major, minor, revision;
  hoedown_version(&major, &minor, &revision);
  Local<Value> args [3] = {NanNew(major), NanNew(minor), NanNew(revision)};
  Local<Object> version = NanNew(Version::constructor)->GetFunction()->NewInstance(3, args);
  target->Set(NanNew("version"), version);
}


void init(Handle<Object> target) {
  NanEscapableScope();

  // create module function
  Document::Hoedown::init(target);
  target = NanNew(Document::Hoedown::constructor)->GetFunction();

  // all the other things
  Autolink::init(target);
  Document::init(target);
  Escape::init(target);
  HTML::init(target);

  // version
  Version::init(target);
  exposeHoedownVersion(target);
}

NODE_MODULE(hoedown, init)
