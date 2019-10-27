Line Drawing - Snapping {#linedrawing4}
=======================================

While AGG supports subpixel accuray via anti-aliasing, it can have
some issues for an image containing a lot of vertical / horizontal
lines or texts.  These lines can be drawn with thicker than intended
line width and lighter than desired color.

The following example illustrates the problem.

Example
-------

tutorial_linedrawing_4.cpp generates a bunch of horizontal lines
of 0.5 in width.  Notice some of the lines occupy two pixels in
height despite the fact the desired linewidth is 0.5.

\image html tutorial_linedrawing_4.png

Solution
--------

For thin lines (usually line width thinner than 5) where such effect
is quite prounced, shift x / y position slightly so that after
subtracting (linewidth / 2), x / y is an integer.
