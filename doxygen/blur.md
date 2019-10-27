Image - Blur {#imageBlur}
=========================

There are two blur classes: agg::stack_blur and agg::recursive_blur.
agg::stack_blur is 3x-5x faster and has a bit limitations, while agg::recursive_blur
is a true Gaussian blur.  The interface is the same for these two blurs.

```cpp
	agg::stack_blur<agg::rgba8, agg::stack_blur_calc_rgba<> > blur;
	blur.blur (pixFmt, radius);
```

```cpp
	agg::recursive_blur<agg::rgba8, agg::recursive_blur_calc_rgba<> > blur;
	blur.blur (pixFmt, radius);
```

The `radius` is roughly equivalent to twice the value of SVG's `stdDeviation`. 

Note that the entire pixFmt is blurred.  However, it is pretty straightforward
to limit blurring to a specifc region of pixFmt.

```cpp
	template<class PixFormat, class BlurType>
	void
	blurImage(PixFormat& pixFmt, int x, int y, int width, int height, BlurType& blur, double radius)
	{
	    agg::rendering_buffer   renBuffer;
	    PixFormat subPixFmt (renBuffer);
	    subPixFmt.attach (pixFmt, x, y , x + width - 1, y + height - 1);
	    blur.blur (subPixFmt, radius);
	}
```

Example
-------

tutorial_blur.cpp does the following. 

1. Just the image icon by itself.
2. The image icon blurred using agg::stack_blur
3. The image icon blurred using agg::recursive_blur
4. The image icon blurred using agg::recursive_blur with dx = 5 and dy = 5.
   Then original image icon was drawn on top.  Effectively it creates a
   shadow effect.

The resulting image is the following.

\image html tutorial_blur.png

An equivalent SVG is

\image html tutorial_blur.svg
