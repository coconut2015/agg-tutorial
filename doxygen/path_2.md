Path - Fill Even-Odd {#path2}
=============================

Corresponding to SVG's ``fill-rule:evenodd``, AGG also has the ability to
fill evenodd.  Just set the filling_rule to agg::fill_even_odd .

```cpp
	ras.filling_rule(agg::fill_even_odd);
	ras.add_path(path);
```

Example
-------

tutorial_path_2.cpp generates an image containing a red star that was filled
using even-odd rule.

\image html tutorial_path_2.png
