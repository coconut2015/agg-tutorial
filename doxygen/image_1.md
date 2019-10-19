Image - Basics {#image1}
========================

Imaging drawing with AGG is a bit complicated.  It works quite similar
to how SVG draws image.  Instead of drawing the source image at the target
image buffer location with specified scaling as in most 2D graphics APIs,
AGG does the reverse lookup of source image pixels needed to draw the
target image pixel.

The following types are needed.

```cpp
	typedef agg::span_interpolator_linear<agg::trans_affine> InterpolatorType;
	typedef agg::image_accessor_clip<PixelFormat> ImageAccessorType;
	typedef agg::span_image_filter_rgba_nn<ImageAccessorType, InterpolatorType> SpanGeneratorType;
	typedef agg::span_allocator<agg::rgba8> SpanAllocatorType;
```

Then we need to compute the reverse lookup matrix.  In the following code,
`matrix` would be the normal parameters needed to draw images using convention
2D graphics API.  `invertMatrix` would be the reverse lookup matrix.

```cpp
	agg::trans_affine matrix;
	matrix.scale(xScale, yScale);
	matrix.translate(x, y);
	
	// from the matrix, compute the invert matrix.  It will
	// be the lookup of src pixels from the dst location.
	agg::trans_affine invertMatrix (matrix);
	invertMatrix.invert();
```

We also need a path to specify the target region to do the reverse lookup.

```cpp
	double rect[] =
	{
	    (double)x, (double)y,
	    (double)x + width, (double)y,
	    (double)x + width, (double)y + height,
	    (double)x, (double)y + height,
	    (double)x, (double)y
	};
	
	// The target region to draw the image.
	SimplePath path(rect, sizeof(rect) / sizeof(double));
```

Finally, hook everything up and draw the image.

```cpp
	agg::rasterizer_scanline_aa<> ras;
	agg::scanline_p8 scanline;

	InterpolatorType interpolator (invertMatrix);
	ImageAccessorType ia (src.ren(), transparentWhiteColor);
	SpanGeneratorType sg (ia, interpolator);
	SpanAllocatorType sa;

	ras.reset();
	ras.add_path(path);
	agg::render_scanlines_aa(ras, scanline, dst, sa, sg);
```

Example
-------

tutorial_image_1.cpp first generates the following image icon.

\image html tutorial_image_icon.png

Then it draws this image with various scaling, translation etc.

\image html tutorial_image_1.png

An equivalent SVG is

\image html tutorial_image_1.svg
