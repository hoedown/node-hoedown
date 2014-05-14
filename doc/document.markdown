# Document parsing

    Stability: 2 - Unstable

Exports the API to render Markdown.


## Handler: hoedown(options)

This handler constructs a renderer-parser pair, and renders arbitrary markdown through it. It's constructed by calling the module itself.

Example:

```js
var renderer = hoedown();
renderer.do("some markdown") //-> "<p>some markdown</p>\n"
```

`options` is an object with the following properties:

  * `extensions` is an integer bitmask describing which unofficial extensions should be recognized and parsed.
    It should be formed by ORing together one or more flags in [`hoedown.Extensions`](#flags_hoedown_extensions).  
    Default: 0 (no extensions, parse pure markdown).

  * `maxNesting` is an integer describing the maximum nesting level to be parsed.
    Spans or blocks nested more times than this setting will be ignored.  
    Default: 16.

  * `renderer` is used to choose the renderer, as well as for renderer-specific options.

    * `type` must be a special object identifying the renderer to use.
       You may pass `hoedown.HTML` for the [HTML renderer](html.markdown#hoedown_html) or `hoedown.HTML.TOC` for the [HTML TOC renderer](html.markdown#hoedown_html_toc).  
       Default: `hoedown.HTML`.
    * Specific options for the choosen renderer, which may be found in its corresponding section.

  * `unit` is a low-level integer parameter. It's the number of bytes that should be (re)allocated when the buffer is full.
    Smaller units mean less memory but more frequent allocations.  
    Default: 64.

  * `minimumSize` is a low-level integer parameter. It's the minimum number of bytes the output buffer is always
    allocated before rendering. It's recommended to set it to a high enough length to contain most outputs.  
    Default: 0 (don't preallocate).

  * `maximumSize` is a low-level integer parameter. It's the maximum number of bytes the output buffer can use
    without being reallocated to `minimumSize` the next render. It should be at least `minimumSize`.  
    Default: `minimumSize` setting.


## Flags: hoedown.Extensions

Describe which *extensions* (modifications to regular Markdown) should be used when parsing.  
They fall into four main categories:

#### Block extensions

Extensions adding support for a new block-level callback.  
`hoedown.EXT_BLOCK` has the combination of all the extensions in this group.

  * `hoedown.Extensions.TABLES`: Parse PHP-Markdown style tables.
  * `hoedown.Extensions.FENCED_CODE`: Parse fenced code blocks.
  * `hoedown.Extensions.FOOTNOTES`: Parse footnotes.

#### Span extensions

Extensions adding support for a new span-level callback.  
`hoedown.EXT_SPAN` has the combination of all the extensions in this group.

  * `hoedown.Extensions.AUTOLINK`: Automatically turn safe URLs into links.
  * `hoedown.Extensions.STRIKETHROUGH`: Parse `~~stikethrough~~` spans.
  * `hoedown.Extensions.UNDERLINE`: Parse `_underline_` instead of emphasis.
  * `hoedown.Extensions.HIGHLIGHT`: Parse `==highlight==` spans.
  * `hoedown.Extensions.QUOTE`: Render `\"quotes\"` as `<q>quotes</q>`.
  * `hoedown.Extensions.SUPERSCRIPT`: Parse `super^script`.

#### Flag extensions

Extensions that modify the behaviour of an existing feature, restricting or making it more permissive.  
`hoedown.EXT_FLAGS` has the combination of all the extensions in this group.

  * `hoedown.Extensions.LAX_SPACING`: Allow HTML blocks on the same line as text.
  * `hoedown.Extensions.NO_INTRA_EMPHASIS`: Disable emphasis_between_words.
  * `hoedown.Extensions.SPACE_HEADERS`: Require a space after '#' in headers.

#### Negative extensions

Extensions that disable the parsing of an extisting feature.  
`hoedown.EXT_NEGATIVE` has the combination of all the extensions in this group.

  * `hoedown.Extensions.DISABLE_INDENTED_CODE`: Don't parse indented code blocks.


## Flags: hoedown.ListFlags

Passed internally to the renderer to indicate various properties of the current list / list item.

  * `hoedown.ListFlags.LIST_ORDERED`: the list is ordered (i.e. numbered); else the list is unordered (i.e. bullet list).
  * `hoedown.ListFlags.LI_BLOCK`: the list item contains other blocks (i.e. paragraphs); else the list item directly contains inline elements and text.


## Flags: hoedown.TableFlags

Passed internally to the renderer to indicate various properties of the current table / table row / table cell.

  * `hoedown.TableFlags.ALIGN_LEFT`: the current column is aligned to the left.
  * `hoedown.TableFlags.ALIGN_RIGHT`: the current column is aligned to the right.
  * `hoedown.TableFlags.ALIGN_LEFT`: the current column is centered.
  * `hoedown.TableFlags.ALIGNMASK`: bits corresponding to the alignment flags (this is not a flag itself, it's used for filtering).
  * `hoedown.TableFlags.HEADER`: the current cell is in the table's header.


## Enum: hoedown.AutolinkType

Passed internally to the renderer to indicate which type of autolinking was
detected in that specific link.

  * `hoedown.AutolinkType.NONE`: internal value.
  * `hoedown.AutolinkType.NORMAL`: normal web URL (`http`, `https`, `ftp`, etc.).
  * `hoedown.AutolinkType.EMAIL`: email address.
