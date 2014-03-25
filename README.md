# node-hoedown
<sup>**Warning:** the API may change a bit until we release v1!</sup>

```js
var hoedown = require("hoedown");
```

Lightweight, fast Node.JS bindings of the [Hoedown][] library,
the successor for the abandoned [Sundown][].

*Really* simple to use:

```js
var renderer = hoedown();

renderer("Some *useful* Markdown.") //-> "<p>Some <em>useful</em> Markdown.</p>"
renderer("¿ÚTF? Nö **prøblem**.") //-> "<p>¿ÚTF? Nö <strong>prøblem</strong>.</p>"
// be sure to reuse the function for blazing speed!
```


## It's flexible

If parsing pure Markdown and generating HTML isn't enough,
you can pass an options object in your call to `hoedown()`.
The object follows this structure:

```js
hoedown({
  // parse (unofficial) Markdown extensions
  extensions: 0,
  // maximum nesting level to be parsed
  maxNesting: 16,

  // settings for the renderer
  renderer: {
    // which renderer to use
    type: hoedown.HTML,
    // renderer-specific options (see below)
  },

  // grow output buffer each X bytes
  unit: 64,
  // grow output buffer to this initial size
  initialSize: 0,

  // whether or not to apply SmartyPants after rendering
  smartypants: false,
})
```

Where `extensions` is a combination of the following flags:

TODO

### HTML renderer

Renders pure (X)HTML. Usage:

```js
hoedown({
  renderer: {
    type: hoedown.HTML,
    // rendering flags 
    flags: 0
    // non-zero value enables TOC links
    // up to the specified header level
    tocLevel: 0
  }
})
```

Where `flags` is a combination of the following flags:

TODO

### HTML TOC renderer

Don't render the document, but a Table of Contents with links pointing
to the document's headers. Usage:

```js
hoedown({
  renderer: {
    type: hoedown.HTMLToc,
    // non-zero value enables TOC links
    // up to the specified header level
    tocLevel: 0
  }
})
```

### Other things

```js
> if (hoedown.version.major > 1) {
... console.log("Hoedown is at version %s.", hoedown.version);
... }
Hoedown is at version 3.0.0.
> TODO
```


## It's secure

Hoedown is all about security. It has been extensively tested to make sure it won't
crash under any input. If you believe you have found a vulnerability,
[report it there][Hoedown bugs].


## No custom renderers?

Users coming from Robotskirt may notice these bindings don't offer the possibility
of including custom callbacks in renderers, or create pure JS renderers. There are
many reasons for that:

 * **Painfully slow:** the constant switches from C++ to JS decrease performance.
 * **Invalid:** the callbacks had to be synchronous, which totally stopped you from
   working with asynchronous libraries, which are common.

If you need further processing, it's better to process the rendered output (if it's
HTML, you can use jsdom for instance).

FIXME: mention JSON

**Tip:** If you need code highlighting, just include [Prism][] in your site,
it'll automatically highlight your fenced code blocks.



[Hoedown]: https://github.com/hoedown/hoedown
[Hoedown bugs]: https://github.com/hoedown/hoedown#help-us
[Sundown]: https://github.com/vmg/sundown
[Prism]: http://prismjs.org
