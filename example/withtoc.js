#!/usr/bin/env node
// Reads Markdown from standard input, and renders HTML
// as normal, but also renders the Table of Contents just
// above the content itself.

var hoedown = require("hoedown");

// Here we pick all block and span extensions, instead of specifying them one by one.
// Hint: you don't do this at production.
var exts = hoedown.EXT_BLOCK | hoedown.EXT_SPAN | hoedown.EXT_SPACE_HEADERS;
// Maximum header level to be included in the TOC
var toc = 4;

// create renderer for the Table of Contents
var rendertoc = hoedown({
  extensions: exts,
  maxNesting: 10,

  renderer: {
    type: hoedown.HTML.TOC,
    tocLevel: toc,
    smartypants: true
  }
});

// create regular HTML renderer
var render = hoedown({
  extensions: exts,

  renderer: {
    type: hoedown.HTML,
    flags: hoedown.HTML.Flags.HARD_WRAP,
    tocLevel: toc,
    smartypants: true
  }
});

collectInput(process.stdin, function(input) {
  // render both TOC and content
  var tochtml = rendertoc(input);
  var html = render(input);

  // output them
  process.stdout.write(tochtml + "\n\n" + html);
});



// logic to collect all the input

function collectInput(stream, callback) {
  req.setEncoding("utf8");
  var data = "";
  stream.on("data", function(chunk) {
    data += chunk;
  });
  stream.on("end", function() {
    callback(data);
  });
}
