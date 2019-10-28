Predefined Shapes - Rectangle {#shape2}
=======================================

Rectangles and rounded rectangles are fairly commonly used shapes.
Just include the following header.

```cpp
	#include <agg_rounded_rect.h>
```

Then you can a rectangle using x1, y1, x2, y2, and radius.  To have different
rx and ry, use the radius function.

```	
	agg::rounded_rect rect2 (20, 60, 170, 110, 10);
```

Note that it is not necessary to use agg::conv_curve to convert path commands.

Example
-------

tutorial_shape_1.cpp draws a very simple ellipse.

\image html tutorial_shape_2.png

An equivalent SVG is

\image html tutorial_shape_2.svg
