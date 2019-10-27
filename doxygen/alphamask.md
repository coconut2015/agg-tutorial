Image - Alpha Mask {#imageAlphaMask}
====================================

You can create an alpha mask for a particular channel of an image.  In the
following case, agg::alpha_mask_rgba32a uses the alpha channel.  It
is possible to use agg::alpha_mask_rgba32r for red channel, and so on.

```cpp
	typedef agg::alpha_mask_rgba32a AlphaMaskType;
	AlphaMaskType alphaMask (iconRenderBuffer);
```
To hook things up, one basically create an alpha mask pixel format
(AMPixelFormat below) that wraps around the original pixel format
with the alpha mask.

```cpp
	typedef agg::pixfmt_amask_adaptor<PixelFormat, AlphaMaskType> AMPixelFormat;
	typedef agg::renderer_base<AMPixelFormat> AMRendererBaseType;
	
	AMPixelFormat amPixFmt (pixFmt, alphaMask);
	AMRendererBaseType amRBase (amPixFmt);
	agg::renderer_scanline_aa_solid<AMRendererBaseType> amrs (amRBase);
	amrs.color(color);
	
	agg::rasterizer_scanline_aa<> ras;
	ras.auto_close(false);
	agg::scanline_p8 scanline;

	ras.reset();
	ras.add_path(path);
	agg::render_scanlines(ras, scanline, amrs);
```

Example
-------

tutorial_alpha.cpp does the following. 

1. Just the image icon by itself.
2. The image icon's alpha channel being drawn as gray using the alpha channel.
3. The image icon's alpha channel being drawn as gray using the alpha channel
   and then blurred.
4. The image icon's alpha channel being drawn as gray using the alpha channel
   at an dx = 5, dy = 5 offset, then blurred.  Then the original image icon
   was drawn on top.  Effectively it creates a gray shadow effect.

The resulting image is the following.

\image html tutorial_alphamask.png

An equivalent SVG is

\image html tutorial_alphamask.svg
