#!/usr/bin/env coffee
# A simple example of use with CoffeeScript.

hoedown = require 'hoedown'


renderer = hoedown(
  extensions: hoedown.Extensions.TABLES |
              hoedown.Extensions.FOOTNOTES |
              hoedown.EXT_SPAN

  renderer:
    flags: hoedown.HTML.Flags.HARD_WRAP
    smartypants: yes
)

html = renderer.do '''

  This is some ---presumably cool--- Markdown.
  It's being rendered by [Hoedown][], the ==amazing== Markdown parser[^1].

  | First name | Last name | Age | Total  |
  |------------|-----------|-----|--------|
  | Peter      | Parker    | 28  |  $9.99 |
  | John       | Hood      | 33  | €21.64 |
  | Clark      | Kent      | 18  | $15.89 |

  [Hoedown]: https://github.com/hoedown/hoedown "Project page"
  [^1]: It's actually based on https://github.com/vmg/sundown.

'''

console.log html
