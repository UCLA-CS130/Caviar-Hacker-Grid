# CS130 Assignment 9

## Implemented Features
* Markdown -> HTML rendering
* S3 Bucket Storage

## Markdown -> HTML Rendering
* This page is written in Markdown!
* To see this page's raw text, [click here](http://richardm.in/CS130/serve/design_doc_markdown.md)

## Design Decisions
* Separate handler, because you may want to have both markdown served as HTML and raw (i.e. github has RAW setting)
    * The other choice here was to make it an option, but would configure for the entire endpoint
* Serve non-markdown files normally because otherwise lots of endpoint switching is involved

## Demo showing markdown features:

# H1 Example
## H2 Example
### H3 Example
#### H4 Example
##### H5 Example
###### H6 Example
``` code block ```

Alt-H1 Style
============

Alt-H2 Style
------------

Emphasis, aka italics, with *asterisks* or _underscores_.

Strong emphasis, aka bold, with **asterisks** or __underscores__.

1. First ordered list item
2. Another item
    * Unordered sub-list. 
1. Actual numbers don't matter, just that it's a number
    1. Ordered sub-list
4. And another item.

> Blockquotes are very handy in email to emulate reply text.
> This line is part of the same quote.

Quote break.

> This is a very long line that will still be quoted properly when it wraps. Oh boy let's keep writing to make sure this is long enough to actually wrap for everyone. Oh, you can *put* **Markdown** into a blockquote. 

Three or more...

---

Hyphens

***

Asterisks

___

Underscores

Markdown Examples courtesy of [this link](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet)