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

  * `minimumSize` is a low-level integer parameter. It's the minimum number of bytes the output buffer is always
    allocated before rendering. It's recommended to set it to a high enough length to contain most outputs.  
    Default: 0 (don't preallocate).

  * `maximumSize` is a low-level integer parameter. It's the maximum number of bytes the output buffer can use
    without being reallocated to `minimumSize` the next render. It should be at least `minimumSize`.  
    Default: `minimumSize` setting.

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

  * `minimumSize` is a low-level integer parameter. It's the minimum number of bytes the output buffer is always
    allocated before rendering. It's recommended to set it to a high enough length to contain most outputs.  
    Default: 0 (don't preallocate).

  * `maximumSize` is a low-level integer parameter. It's the maximum number of bytes the output buffer can use
    without being reallocated to `minimumSize` the next render. It should be at least `minimumSize`.  
    Default: `minimumSize` setting.

Example:

```js
var escaper = hoedown.escapeHref();
escaper.do("2+2 is four.") //-> "2+2%20is%20four."
```
