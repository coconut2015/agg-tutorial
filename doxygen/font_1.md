Font - Basics {#font1}
======================

AGG draws font glyphs just like any path objects.  Because of AGG's subpixel accuracy,
that is all you need.

On linux, AGG uses [FreeType](https://www.freetype.org/) library to get the font
glyphs.  It can be a bit tricky to get both FreeType and the right libpng installed.
You may have to either upgrade the system or do custom compilation.

Loading a font is straight forward.  See the code below.  It should be noted that
different Linux systems organize TrueType fonts differently.  For this tutorial,
we will just use DejaVuSerif.ttf in fonts/ directory.

```cpp
	agg::font_engine_freetype_int32 fontEngine;
	agg::font_cache_manager<agg::font_engine_freetype_int32> fontCache (fontEngine);

	// load font outline path
	fontEngine.load_font(fontPath, 0, agg::glyph_ren_outline);
	// enable hint
	fontEngine.hinting(true);
	// flip y since the image coordinate system has the origin at
	// top left corner and y axis is flipped
	fontEngine.flip_y(true);
	// set font size
	fontEngine.height(fontSize);
```
For actual text drawing, it is straightforward.  Just get the glyph and draw
the path, depending on the path type.

```cpp
	void
	drawText (const char* str,
	          double x,
	          double y,
	          bool kerning,
	          agg::font_engine_freetype_int32& fontEngine,
	          agg::font_cache_manager<agg::font_engine_freetype_int32>& fontCache,
	          agg::rgba8 color,
	          RendererBaseType& rBase)
	{
	    bool first = true;
	
	    // these three are only needed for outline rendering
	    agg::conv_curve<agg::font_engine_freetype_int32::path_adaptor_type> curves(fontCache.path_adaptor());
	    agg::rasterizer_scanline_aa<> ras;
	    agg::scanline_p8 scanline;
	
	    for (; *str != 0; ++str)
	    {
	        unsigned ch = *str;
	        const agg::glyph_cache* glyph = fontCache.glyph(ch);
	        if (glyph)
	        {
	            if (first)
	            {
	                first = false;
	            }
	            else
	            {
	                if (kerning)
	                {
	                    fontCache.add_kerning(&x, &y);
	                }
	            }
	            fontCache.init_embedded_adaptors(glyph, x, y);
	            switch (glyph->data_type)
	            {
	                case agg::glyph_data_mono:
	                {
	                    agg::render_scanlines_bin_solid(fontCache.mono_adaptor(), fontCache.mono_scanline(), rBase, color);
	                    break;
	                }
	                case agg::glyph_data_gray8:
	                {
	                    agg::render_scanlines_aa_solid(fontCache.gray8_adaptor(), fontCache.gray8_scanline(), rBase, color);
	                    break;
	                }
	                case agg::glyph_data_outline:
	                {
	                    ras.reset();
	                    ras.add_path(curves);
	                    agg::render_scanlines_aa_solid(ras, scanline, rBase, color);
	                    break;
	                }
	                default:
	                {
	                    throw TutorialException ("Unknown glyph type: %d", glyph->data_type);
	                }
	            }
	            x += glyph->advance_x;
	            y += glyph->advance_y;
	        }
	    }
	}
```

Example
-------

tutorial_font_1.cpp generates three rows of "Hello World!" text using different
types of glyph rendering.

1. agg::glyph_ren_agg_mono
2. agg::glyph_ren_agg_gray8
3. agg::glyph_ren_outline

\image html tutorial_font_1.png
