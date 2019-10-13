Font - Advanced {#font2}
========================

Using font glyphs as path objects, it is possible to use them besides
drawing plain texts.

In this tutorial, drawImage function in \ref image2 is modified a
bit to take on different fill rules.

And instead of directly drawing glyphs immediately, they are saved
to agg::path_storage.  Then we pass the generated path to drawImage
function.

Example
-------

tutorial_font_2.cpp uses font path as clip region to draw images.

\image html tutorial_font_2.png
