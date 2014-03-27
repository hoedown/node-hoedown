#!/usr/bin/env node
// The original `hoedown.smartypants` function parses HTML and returns the resulting
// HTML with entities denoting the punctuation characters. Instead, this script accepts
// and applies SmartyPants to plain text.
//
//     $ ./smartypants.js <<< "Jon 'said' 4 < 5 and---you guessed it---laughed again..."
//     Jon ‘said’ 4 < 5 and—you guessed it—laughed again…
//

var hoedown = require("hoedown");

collectInput(process.stdin, function(text) {
  // escape all the input, or smartypants will interpret it as HTML
  text = hoedown.escapeHTML(text);
  // perform smartypants
  text = hoedown.smartypants(text);
  // unescape the input to get plain text again
  text = hoedown.unescapeHTML(text);

  process.stdout.write(text);
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
