# Escaping

    Stability: 2 - Unstable

Exports Hoedown's escaping utilities.


## Handler: hoedown.escapeHTML(options)

Returns a handler that escapes HTML.  
If provided, `options` should have these properties:

  * `secure` is a boolean. If true, additional characters are escaped for extra security.  
    Default: `false`.

  * `unit` is a low-level integer parameter. It's the number of bytes that should be (re)allocated when the buffer is full.
    Smaller units mean less memory but more frequent allocations.  
    Default: 64.

  * `initialSize` is a low-level integer parameters. It's the number of bytes that should be initially allocated before doing any renders.  
    Default: 0.

Example:

```js
var escaper = hoedown.escapeHTML({ initialSize: 2000, secure: true });
escaper.do("SOME <REALLY> LONG HTML") //-> "SOME &lt;REALLY&gt; LONG HTML"
```


## Handler: hoedown.escapeHref(options)

Returns a handler that escapes URLs with the percentage form.  
If provided, `options` should have these properties:

  * `unit` is a low-level integer parameter. It's the number of bytes that should be (re)allocated when the buffer is full.
    Smaller units mean less memory but more frequent allocations.  
    Default: 64.

  * `initialSize` is a low-level integer parameters. It's the number of bytes that should be initially allocated before doing any renders.  
    Default: 0.

Example:

```js
var escaper = hoedown.escapeHref();
escaper.do("2+2 is four.") //-> "2+2%20is%20four."
```
