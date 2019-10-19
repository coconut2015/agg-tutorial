Line Drawing - Dashed Line {#linedrawing3}
==========================================

Much like curved lines, the way dashed line works is by having agg::conv_dash
to generate a line path from the line path passed in.

```cpp
	agg::conv_dash<CmdVertexPath> dashPath (path);
	// specify dash pattern
	dashPath.add_dash(3 * strokeWidth, 3 * strokeWidth);
	dashPath.add_dash(1 * strokeWidth, 3 * strokeWidth);
```

It can take some efforts to create the correct dash pattern.

Example
-------

tutorial_linedrawing_3.cpp generates an image containing a caret and a dome
with dash dot lines.

\image html tutorial_linedrawing_3.png

An equivalent SVG is

\image html tutorial_linedrawing_3.svg
