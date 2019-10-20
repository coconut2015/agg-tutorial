Line Drawing - Solid Line {#linedrawing1}
=========================================

AGG does not really have a line drawing routine in the traditional 2D graphics
API sense.  Instead, it generates polygons from the line paths and strokes.
Then fill the polygons.

The following codes generate a solid line of width 1.0 for the path provided.
Its properties are very similar to that of SVG's polyline.

```cpp
	agg::conv_stroke<SimplePath> strokePath (path);
	
	double strokeWidth = 1.0;
	strokePath.width(strokeWidth);
	strokePath.line_cap(agg::square_cap);
	strokePath.line_join(agg::miter_join);
	strokePath.miter_limit(strokeWidth);
```

It should be noted that this path will treat any agg::path_cmd_curve3,
agg::path_cmd_curve4 etc as simply agg::path_cmd_line_to.  We will
deal with those in the next section.

Example
-------

tutorial_linedrawing_1.cpp generates an image containing a green border and star.

\image html tutorial_linedrawing_1.png

An equivalent SVG is

\image html tutorial_linedrawing_1.svg
