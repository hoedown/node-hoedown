# node-hoedown <sup>(beta)</sup>

[![Build Status](https://travis-ci.org/hoedown/node-hoedown.png?branch=master)](https://travis-ci.org/hoedown/node-hoedown)

```js
var hoedown = require("hoedown");
```

Lightweight, fast Node.JS bindings for [Hoedown][], the amazing Markdown library (successor for the abandoned [Sundown][]).

*Really* simple to use:

```js
var renderer = hoedown();

renderer.do("Some *useful* Markdown.") //-> "<p>Some <em>useful</em> Markdown.</p>"
renderer.do("¿ÚTF? Nö **prøblem**.") //-> "<p>¿ÚTF? Nö <strong>prøblem</strong>.</p>"
// be sure to reuse the object for blazing speed!
```


## It's flexible

Because parsing pure Markdown is really boring, you can pass an options object to
`hoedown()` to customize it. How about making it recognize some extensions?

```js
var renderer = hoedown({
  extensions: hoedown.Extensions.AUTOLINK | hoedown.Extensions.FOOTNOTES
});

renderer.do("Here's a http://link.com.") //-> "<p>Here's a <a href="http://link.com">http://link.com</a>.</p>"
```

You can see the full list of extensions in [the docs](doc/document.markdown#flags_hoedown_extensions).  
It's also possible to customize the HTML renderer by passing some [flags](doc/html.markdown#flags_hoedown_html_flags):

```js
var renderer = hoedown({
  extensions: hoedown.Extensions.AUTOLINK | hoedown.Extensions.FOOTNOTES,
  renderer: {
    flags: hoedown.HTML.Flags.HARD_WRAP | hoedown.HTML.Flags.ESCAPE
  }
});

renderer.do("Roses are red.\nViolets are blue.") //-> "<p>Roses are red.<br>Violets are blue.</p>"
```

The full options accepted by the HTML renderer can be found at [the docs](doc).
You can also use the TOC renderer to render a Table of Contents of the document,
as in [this example](example/withtoc.js).

#### Other things

Hoedown comes with a fully functional implementation of [SmartyPants][].
You can also access Hoedown's version, use the autolinker, escape things, etc.
Check out the [docs](doc) and the [examples](example)!


## It's secure

Hoedown is all about security. It has been extensively tested to make sure it won't
crash or leak under any input. If you believe you have found a vulnerability,
[report it there][Hoedown bugs].

That said, the HTML produced by Hoedown is *not* secure. Unless you're using the
`ESCAPE` or `SKIP_HTML` flags, you should really consider sanitizing the HTML.


## No custom renderers?

Users coming from Robotskirt may notice these bindings don't offer the possibility
of including custom callbacks in renderers, or create pure JS renderers. There are
many reasons for that, notably:

 * **Painfully slow:** the constant switches between C++ and JS decrease performance.
 * **Often useless:** the callbacks had to be synchronous, which totally stopped you from
   working with asynchronous libraries.

If you need further processing, it's better to process the rendered output (if it's
HTML, you can use jsdom for instance). FIXME: mention JSON

**Tip:** If you need code highlighting, just include [Prism][] in your site,
it'll automatically highlight your fenced code blocks.



[Hoedown]: https://github.com/hoedown/hoedown
[Hoedown bugs]: https://github.com/hoedown/hoedown#help-us
[Sundown]: https://github.com/vmg/sundown
[Prism]: http://prismjs.com
[SmartyPants]: http://daringfireball.net/projects/smartypants
