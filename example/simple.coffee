#!/usr/bin/env coffee
# Simple usage example for CoffeeScript.

hoedown = require "hoedown"
{ Extensions, HTML } = hoedown


# Initialize a renderer
myRenderer = hoedown {
  extensions: [ Extensions.TABLES
                Extensions.FENCED_CODE
                Extensions.AUTOLINK
                Extensions.NO_INTRA_EMPHASIS ]

  renderer:
    type: HTML
    flags: [ HTML.Flags.HARD_WRAP
             HTML.Flags.SKIP_HTML ]
    smartypants: yes
}


# Render stuff! Yay!
html = myRenderer.do '''

  CoffeeScript is beautiful.
  But Markdown *inside* CoffeeScript is just sublime.

  Look at how simple it is to render custom Markdown ---with smart punctuation!---
  using Hoedown. You can reuse it multiple times and it's pretty fast...


  ## The killer part

  A CoffeeScript code block, inside a Markdown string, inside CoffeeScript.
  Don't argue me on how meta this is.

  ~~~ coffeescript
  # Greatest Common Divisor
  gcd = (a, b) ->
    if a is 0 then return b
    gcd b, a%b

  gcd 60, 24  #-> 12
  ~~~

'''

# Log the rendered HTML
console.log html
