# Hoedown

```js
    var hoedown = require("hoedown");
```

Hoedown is a Markdown processing library that aims to be fast and efficient,
while customizable at the same time. This module exports the whole Hoedown
API in a simple way, abstracting the low-level details.

Unlike its predecessor [Robotskirt][], this doesn't implement the required
introspection to supply JS callbacks to Hoedown, in order to customize the
output. This was found to be pretty slow, due to the constant switches
between JS and C, and callbacks had to return synchronously, which limited
their practical use.


## Handlers
<!-- type=misc -->

Handlers can be found around the API. They are little functions, which are
invoked with an (optional) options object, and return an object with the only
purpose of processing an input string and returning the resulting string.

As explained in [document](document.markdown) later, the module itself is a
handler. It's called with an options object, and it returns an object that can
be called with markdown, and it returns the rendered HTML.

```js
var renderer = hoedown({ ... });
renderer.do("some markdown") //-> "<p>some markdown</p>\n"
```

There are other hanlders, too:

```js
var escaper = hoedown.escapeHref();
escaper.do("a URL") //-> "a%20URL"
```

Handlers were made to be reused. They don't deallocate the output buffer, which
means they reuse the same memory over and over to render each input.
They have the following methods:

### do(input)

Processes the input and returns the result as a String.


## Main sections

Hoedown is modular enough to export its internal utilities, such as escaping,
autolinking, or sanitizing, in separate headers. Each section talks about
one specific set of features, which doesn't necessarily mean they're namespaced.

 * [**Document parsing**](document.markdown)
 * [**HTML**](html.markdown)
 * [**Escaping**](escape.markdown)
 * [**Autolinking**](autolink.markdown)


## Class: hoedown.Version

A simple class representing a three-field version.

### version.major
Integer: The major version number.

### version.minor
Integer: The minor version number.

### version.revision
Integer: The revision version number.

### version.toString()

Outputs a readable string representing the version, which is formed of the major, minor
and revision versions separated by dots.

Example:

```js
var v = hoedown.Version(2,7,1);
v.toString() //-> "2.7.1"
```

### version.inspect()

Method to provide a better representation of the version object when it is inspected
by `util.inspect()`.

Example:

```js
var v = new hoedown.Version(2,7,1);
util.inspect({ myversion: v })
//-> "{ myversion: <Version 2.1.2> }"
```


## hoedown.version

Holds the version of the native library (Hoedown).  
Keep in mind, this is not the module's NPM version.

Example:

```js
console.log("Hoedown is at version %s.", hoedown.version);
// Hoedown is at version 3.0.0.
```



[Robotskirt]: https://github.com/benmills/robotskirt
