This directory contains `.markdown` files describing the API
of the module, the same way [Node's official documentation]
(https://github.com/joyent/node/tree/master/doc/api) is
written. Each markdown file usually has a 1:1 relationship
with its corresponding `.hpp` file, not to the API.

This means in theory one should be able to use the [same tool]
(https://github.com/joyent/node/tree/master/tools/doc) (with
a few modifications) to generate HTML and JSON for regular use.

Start reading on [`index.markdown`](index.markdown).
