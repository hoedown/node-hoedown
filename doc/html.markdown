# HTML

    Stability: 2 - Unstable

Exports Hoedown's HTML renderer and other HTML-related utilities.


## hoedown.HTML

The default HTML renderer.  
It accepts the following options in the `renderer` object:

  * `flags` is an integer bitmask formed by a combinations of the flags in [hoedown.HTML.Flags](#flags_hoedown_html_flags) to alter renderer behaviour.  
    Default: 0 (no flags).

  * `smartypants` is a boolean. If true, the resulting HTML will internally be passed to SmartyPants (see [below](#handler_hoedown_html_smartypants)) for smart punctuation.  
    Default: `false`.

  * `tocLevel` is an integer. If it is non-zero, anchors will be put on every header whose level isn't greater than this parameters.
    This is usually used together with the [TOC renderer](#hoedown_html_toc) to produce a Table of Contents with links to the headers.  
    Default: 0 (don't produce anchors).

Example:

```js
var renderer = hoedown({
  renderer: {type: hoedown.HTML, smartypants: true}
});

renderer.do("Hey there, I'm SmartyPanted HTML!")
//-> "<p>Hey there, I&rsquo;m SmartyPanted HTML!</p>\n"
```


## hoedown.HTML.TOC

A renderer that produces a Table of Contents of the document in HTML.  
That is, nested lists of links pointing to the headers.

It accepts the following options in the `renderer` object:

  * `smartypants` is a boolean. If true, the resulting HTML will internally be passed to SmartyPants (see below) for smart punctuation.  
    Default: `false`.

  * `tocLevel` is an integer. If it is non-zero, anchors will be put on every header whose level isn't greater than this parameters.
    This is usually used together with the [TOC renderer](#hoedown_html_toc) to produce a Table of Contents with links to the headers.  
    Default: 0 (don't produce anchors).

See [`smartypants.js`](../example/smartypants.js) for an example.

You should be consistent and apply the same options when rendering this TOC and the document itself (especially `tocLevel`) for the Table of Contents to match the document's content.
It's best to create the two renderers with the same options, changing only `renderer.type`.


## Flags: hoedown.HTML.Flags

Flags which modify the behaviour of the HTML renderer.

  * `hoedown.HTML.Flags.SKIP_HTML`: Strip all HTML tags.
  * `hoedown.HTML.Flags.SKIP_STYLE`: Strip `<style>` tags.
  * `hoedown.HTML.Flags.SKIP_IMAGES`: Don't render images.
  * `hoedown.HTML.Flags.SKIP_LINKS`: Don't render links.
  * `hoedown.HTML.Flags.EXPAND_TABS`: Expand tabs to spaces.
  * `hoedown.HTML.Flags.SAFELINK`: Only allow links to safe protocols.
  * `hoedown.HTML.Flags.TOC`: Produce links to the Table of Contents.
  * `hoedown.HTML.Flags.HARD_WRAP`: Render each linebreak as `<br>`.
  * `hoedown.HTML.Flags.USE_XHTML`: Render XHTML.
  * `hoedown.HTML.Flags.ESCAPE`: Escape all HTML.


## Handler: hoedown.HTML.smartypants(options)

Applies [SmartyPants][] to arbitrary HTML.
SmartyPants automatically transforms ASCII characters like `--`, `...`, `""` into its corresponding punctuation symbols: &ndash;, &hellip;, &ldquo;&rdquo;, etc.

It's often used together with markdown; if you want to do that, just pass `smartypants: true` to the HTML renderer and your markdown will be SmartyPanted as well.

This handler is provided in case you need to SmartyPant other HTML.  
If provided, `options` should have these properties:

  * `unit` is a low-level integer parameter. It's the number of bytes that should be (re)allocated when the buffer is full.
    Smaller units mean less memory but more frequent allocations.  
    Default: 64.

  * `minimumSize` is a low-level integer parameter. It's the minimum number of bytes the output buffer is always
    allocated before rendering. It's recommended to set it to a high enough length to contain most outputs.  
    Default: 0 (don't preallocate).

  * `maximumSize` is a low-level integer parameter. It's the maximum number of bytes the output buffer can use
    without being reallocated to `minimumSize` the next render. It should be at least `minimumSize`.  
    Default: `minimumSize` setting.

Example:

```js
var sps = hoedown.HTML.smartypants();
sps.do("Hey ---said the <strong>cow</strong>--- just let me use SmartyPants!")
//-> "Hey &mdash;said the <strong>cow</strong>&mdash; just let me use SmartyPants!"
```


## hoedown.HTML.isTag(html, tagname)

Check if `html` immediately starts with an HTML tag of name `tagname`, and return the appropiate value of [hoedown.HTML.TagType](#enum_hoedown_html_tagtype).

Example:

```js
hoedown.HTML.isTag('<mytag attr1="value"> Some <b>HTML</b>', "mytag") == hoedown.HTML.TagType.OPEN //-> true
hoedown.HTML.isTag(' <mytag> wrong.', "mytag") == hoedown.HTML.TagType.OPEN //-> false
hoedown.HTML.isTag('<mytag>', "othertag") == hoedown.HTML.TagType.NONE //-> true
hoedown.HTML.isTag('</mytag>', "mytag") == hoedown.HTML.TagType.CLOSE //-> true
```


## Enum: hoedown.HTML.TagType

Values for identifying the type of HTML tag present.

  * `hoedown.HTML.TagType.NONE` (0): there's no matching tag.
  * `hoedown.HTML.TagType.OPEN`: it's an opening tag (element begins).
  * `hoedown.HTML.TagType.CLOSE`: it's a closing tag (element ends).



[SmartyPants]: http://daringfireball.net/projects/smartypants
