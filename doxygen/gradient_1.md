Gradient - Gradient Functions {#gradient1}
==========================================

AGG provides plenty of way of generating color gradients.  There are
two components in drawing a color gradient:

1. gradient function, which generates a scalar intensity value based on
   the current x, y position.
2. color function, which generates a color based on the intensity value.

For this tutorial, we will play around with various gradient functions.
However, for the color function, we will use a simple agg::gradient_linear_color
object.

You will need the following types.  There are a number of gradient function
classes.  In this example, we use agg::gradient_x. 

```cpp
	typedef agg::gradient_x GradientFunctionType;
	typedef agg::gradient_linear_color<agg::rgba8> ColorFunctionType;
	typedef agg::span_interpolator_linear<agg::trans_affine> InterpolatorType;
	typedef agg::span_allocator<agg::rgba8> SpanAllocatorType;
	typedef agg::span_gradient<agg::rgba8,
	                           InterpolatorType,
	                           GradientFunctionType,
	                           ColorFunctionType> SpanGradientType;
```

Now we create objects for each type and hook everything up.

Note that for the matrix used for the linear interpolator, it is the inverted
matrix for reverse lookup.  This is quite similar to how images are
drawn (\ref image1).


```cpp
	agg::trans_affine invertMatrix (matrix);
	invertMatrix.invert();
	InterpolatorType interpolator (invertMatrix);

	GradientFunctionType gradientFunction;	
	ColorFunctionType colorFunction (agg::rgba8 (0, 0, 0, 0xff),
	                                 agg::rgba8 (0xff, 0xff, 0xff, 0xff),
	                                 100);
	
	SpanGradientType sg (interpolator, gradientFunction, colorFunction, 0.0, 50.0);
	SpanAllocatorType sa;
	
	agg::rasterizer_scanline_aa<> ras;
	agg::scanline_p8 scanline;
	
	ras.auto_close(false);
	ras.reset ();
	TransformPath<SimplePath> tp (path, matrix);
	ras.add_path(tp);
	agg::render_scanlines_aa(ras, scanline, rBase, sa, sg);
```

Example
-------

tutorial_gradient_1.cpp generates 3 rows of gradient functions.  The first
row have a bunch of squares that are generated using the following gradient
functions.

1. agg::gradient_x
2. agg::gradient_y
3. agg::gradient_diamond
4. agg::gradient_xy
5. agg::gradient_sqrt_xy
6. agg::gradient_conic
7. agg::gradient_circle
8. agg::gradient_radial
9. agg::gradient_radial_d
10. agg::gradient_radial_focus

The second row combines agg::gradient_repeat_adaptor with the gradient
functions in the first row.

The third row combines agg::gradient_reflect_adaptor with the gradient
functions in the first row.

\image html tutorial_gradient_1.png
