Path - Fill {#path1}
====================

AGG does most of its operations based on paths.  A path object needs to have the following two functions implemented.

```cpp
	void rewind (int path = 0);
	unsigned vertex(double* x, double* y);
```

See SimplePath for an example.

Additionally, two objects are needed.  These two objects can be re-used.

* agg::rasterizer_scanline_aa is used to store the paths passed in.
* agg::scanline_p8 is used to store intermediate calcuation results.

Finally, agg::render_scanlines_aa_solid is used to render the paths using scanline algorithm.

```cpp
	agg::rasterizer_scanline_aa<> ras;
	agg::scanline_p8 scanline;
	
	ras.reset();
	ras.add_path(path);
	
	agg::render_scanlines_aa_solid(ras, scanline, rBase, greenColor);
```

The above for antialiased drawing.  If you want to use non-antialiased
drawing, use the following instead.

* Class agg::scanline_bin
* Function agg::render_scanlines_bin_solid

Example
-------

tutorial_path_1.cpp generates an image containing a green square and a red star.

\image html tutorial_path_1.png
