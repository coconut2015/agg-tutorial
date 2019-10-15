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

This tutorial was tested on Ubuntu 18.04 LTS, but the code should be
fairly portable on most other platforms.

You will need the following packages to build and run the tutorial examples.

* cmake
* g++
* libpng
* freetype

Links
-----

* This tutorial's GitHub repo: https://github.com/coconut2015/agg-tutorial
* This tutorial's web page: https://coconut2015.github.io/agg-tutorial
* AGG-2.4 unofficial mirror: https://github.com/coconut2015/agg-2.4 

License
-------

All source codes in this tutorial are licensed under APL 2.0.

Donate
------

If you like this tutorial, consider buying me a coffee.

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=3VR7Z5XMLMDP4&currency_code=USD&source=url)

\htmlonly
<form action="https://www.paypal.com/cgi-bin/webscr" method="post" target="_top">
<input type="hidden" name="cmd" value="_donations" />
<input type="hidden" name="business" value="3VR7Z5XMLMDP4" />
<input type="hidden" name="currency_code" value="USD" />
<input type="image" src="https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif" border="0" name="submit" title="PayPal - The safer, easier way to pay online!" alt="Donate with PayPal button" />
<img alt="" border="0" src="https://www.paypal.com/en_US/i/scr/pixel.gif" width="1" height="1" />
</form>
 \endhtmlonly
 