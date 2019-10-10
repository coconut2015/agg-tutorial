Path - Polygon with Holes {#path3}
==================================

To draw a polygon with holes, it is actually not all that much different from
drawing regular polyons.  The only thing needs to be dealt with is the curve
orientation.

If the outer polygon's path is specified in clockwise (CW) ordering,
then the inner hole paths must be specified in counter-clockwise (CCW) ordering.
Likewise, if the outer polygon's path is CCW, then inner hole paths need to
be CW.  If not, then the holes will be filled.

The algorithm that determines the curve orientation is pretty simple.  See
https://en.wikipedia.org/wiki/Curve_orientation

An alternative possible approach is to use the even-odd filling.

SVG's path element has the similar behavior.

Example
-------

tutorial_path_3.cpp generates an image containing red square with a hole.

\image html tutorial_path_3.png
