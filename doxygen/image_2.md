Image - Path and Transformation {#image2}
=========================================

In \ref image1, the image is drawn in a square, which is not all that
interesting. This tutorial should demonstrates the potential of AGG's
image drawing API.

The `drawImage` function is changed a bit.  Instead of
x/y/width/height, it takes the transformation matrix and path.  Then
all we needed to do is to use some creative matrix and shape when
drawing the image.

Example
-------

tutorial_image_2.cpp first generates the following image icon.

\image html tutorial_image_icon.png

Then it draws this image three times.

1. The original image.
2. The original image with a hole in the middle.
3. The original image sheared and with whole in the middle.

\image html tutorial_image_2.png

An equivalent SVG is

\image html tutorial_image_2.svg
