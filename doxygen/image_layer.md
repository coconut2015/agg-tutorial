Image - Layers {#imageLayer}
============================

If it is desired to draw images as separate layers, and then compose them
together as a single image, the layers other than the bottom most layer
needs to use a pixel format with the suffix `_pre`, such as agg::pixfmt_rgba32_pre.

So technically, the previous tutorials ( \ref image1 and \ref image2 )
are not using the correct pixel format for the color wheel image icons.
We more or less got away since the image icon itself does not have many
transparent pixels and they are white to make them really noticeable.

The difference can be seen in the following example.

Example
-------

tutorial_layer.cpp tests the effect of drawing different image types
on the bottom layer.

The top left green square is drawn directly on top the bottom layer.

The top right green square is drawn on a layer with agg::pixfmt_rgba32
pixel format, which is then drawn on the bottom layer.

The bottom left green square is drawn on a layer with agg::pixfmt_rgba32_pre
pixel format, which is then drawn on the bottom layer.

Clearly, the top right green square color is incorrect.

\image html tutorial_layer.png
