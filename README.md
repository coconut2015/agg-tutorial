Anti-Grain Geometry Tutorial {#mainpage}
========================================

Anti-Grain Geometry (AGG) is a wellknown low level graphics library that can
produce very high quality 2D images.  There are many projects use it as
the backend engine.

This tutorial was created because AGG was somewhat difficult to use.  One
basically has to look at other open source projects (e.g. matplotlib)
to figure out how to use some of its functionalities.

This tutorial is based on AGG-2.4, which has a permissive BSD-license.
For the sake of ease of use, I also created an unofficial mirror at
https://github.com/coconut2015/agg-2.4 that strips away most non-essential
files.

AGG Capabilities
----------------

AGG supports most operations in [SVG](https://www.w3.org/Graphics/SVG/).
In fact, it operates in a very similar fashion.  Thus, it is very
straightfoward to create a higher level API that either generates
raster images or SVG files.

For this reason, parts of tutorials will refer to SVG's capability
from time-to-time.

Requirements
------------

This tutorial was tested on Ubuntu 16.04 LTS, but the code should be
fairly portable on most other platforms.  The only change you may have to
do is the font path, which is hardcoded for simplicity.

You will need the following packages to build and run the tutorial examples.

* cmake
* g++
* freetype2
* DejaVu fonts
* libpng

@page tutorials
@page references
