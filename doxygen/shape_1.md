Predefined Shapes - Ellipse {#shape1}
=====================================

Ellipses are commonly used shapes.  Just include the following header.

```cpp
	#include <agg_ellipse.h>
```

Then you can specify a ellipse using x, y, rx, and ry.  There are additional
parameters which you can customize.

```	
	agg::ellipse eliipse (100, 100, 50, 80);
```

Note that it is not necessary to use agg::conv_curve to convert path commands.

Example
-------

tutorial_shape_1.cpp draws a very simple ellipse.

\image html tutorial_shape_1.png

An equivalent SVG is

\image html tutorial_shape_1.svg
