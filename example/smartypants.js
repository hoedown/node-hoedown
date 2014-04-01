#!/usr/bin/env node
// The original `hoedown.smartypants` function parses HTML and returns the resulting
// HTML with entities denoting the punctuation characters. Instead, this script accepts
// and applies SmartyPants to plain text, returning the corresponding HTML.
//
//     $ ./smartypants.js <<< "Jon 'said' 4 < 5 and---you guessed it---laughed again..."
//     Jon &lsquo;said&rsquo; 4 &lt; 5 and&mdash;you guessed it&mdash;laughed again&hellip;
//

var hoedown = require("..");

var escaper = hoedown.escapeHTML();
var smartypanter = hoedown.HTML.smartypants();

collectInput(process.stdin, function(text) {
  // escape all the input, or smartypants will interpret it as HTML
  text = escaper.do(text);
  // perform smartypants
  text = smartypanter.do(text);

  process.stdout.write(text);
});



// logic to collect all the input

function collectInput(stream, cb) {
  stream.setEncoding("utf8");
  var data = "";
  stream.on("data", function(chunk) { data += chunk; });
  stream.on("end", function() { cb(data); });
}
