Line Drawing - Curved Line {#linedrawing2}
==========================================

In order for agg::path_cmd_curve3, agg::path_cmd_curve4 etc to take effect,
it is necessary to add an extra step that compute the actual curve path
using agg::conv_curve.  Then use this curve path to generate the stroke path.

There are a number of advantages with this approach.

* One does not have to pay the cost of generating curves when there are
  no curves in the path.
* It is easy to add / combine path generators depending on the need.

```cpp
	agg::conv_curve<CmdVertexPath> curvePath (path);
	agg::conv_stroke<agg::conv_curve<CmdVertexPath>> strokePath (curvePath);
```

Example
-------

tutorial_linedrawing_2.cpp generates an image containing a caret and a dome.

\image html tutorial_linedrawing_2.png
