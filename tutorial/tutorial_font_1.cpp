/*
 * Copyright (c) 2019 Heng Yuan
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <agg_conv_curve.h>
#include <agg_pixfmt_rgba.h>
#include <agg_rasterizer_scanline_aa.h>
#include <agg_renderer_base.h>
#include <agg_renderer_scanline.h>
#include <agg_scanline_p.h>
#include <agg_font_freetype.h>

#include "path.h"
#include "writepng.h"

typedef agg::pixfmt_rgba32  PixelFormat;
typedef agg::renderer_base<PixelFormat> RendererBaseType;

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

int
main (int argc, const char* argv[])
{
    try
    {
        const int imageWidth = 550;
        const int imageHeight = 300;

        const int pixelSize = PixelFormat::pix_width;

        agg::rendering_buffer   renderBuffer;
        PixelFormat             pixFmt;
        RendererBaseType        rBase;

        // Allocate a 100 x 100 rgba buffer
        unsigned char *imageBuffer = new unsigned char[imageWidth * imageHeight * pixelSize];

        // Hook everything up
        renderBuffer.attach (imageBuffer, imageWidth, imageHeight, imageWidth * pixelSize);
        pixFmt.attach(renderBuffer);
        rBase.attach(pixFmt);

        const agg::rgba8 transparentwhiteColor (0xff, 0xff, 0xff, 0);
        const agg::rgba8 blueColor (0, 0, 0xff, 0xff);

        // clear the buffer with transparent white color
        rBase.clear(transparentwhiteColor);

        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_p8 scanline;

        ras.auto_close(false);

        agg::font_engine_freetype_int32 fontEngine;
        agg::font_cache_manager<agg::font_engine_freetype_int32> fontCache (fontEngine);

        char text[] = "Hello World!";

        // hard code path for simplicity.
        char fontPath[] = "../fonts/DejaVuSerif.ttf";
        double fontSize = 80;
        bool fontHint = true;
        bool fontKerning = true;

        // mono
        {
            if (!fontEngine.load_font(fontPath, 0, agg::glyph_ren_agg_mono))
            {
                throw TutorialException ("Font is not found.");
            }

            // enable font hint
            fontEngine.hinting(fontHint);
            // flip y since the image coordinate system has the origin at
            // top left corner and y axis is flipped
            fontEngine.flip_y(true);
            // set font size
            fontEngine.height(fontSize);
            drawText (text,
                      10, 80,
                      fontKerning,
                      fontEngine,
                      fontCache,
                      blueColor,
                      rBase);
        }

        // gray8
        {
            if (!fontEngine.load_font(fontPath, 0, agg::glyph_ren_agg_gray8))
            {
                throw TutorialException ("Font is not found.");
            }

            // enable font hint
            fontEngine.hinting(fontHint);
            // flip y since the image coordinate system has the origin at
            // top left corner and y axis is flipped
            fontEngine.flip_y(true);
            // set font size
            fontEngine.height(fontSize);
            drawText (text,
                      10, 180,
                      fontKerning,
                      fontEngine,
                      fontCache,
                      blueColor,
                      rBase);
        }

        // outline
        {
            if (!fontEngine.load_font(fontPath, 0, agg::glyph_ren_outline))
            {
                throw TutorialException ("Font is not found.");
            }

            // enable font hint
            fontEngine.hinting(fontHint);
            // flip y since the image coordinate system has the origin at
            // top left corner and y axis is flipped
            fontEngine.flip_y(true);
            // set font size
            fontEngine.height(fontSize);
            drawText (text,
                      10, 280,
                      fontKerning,
                      fontEngine,
                      fontCache,
                      blueColor,
                      rBase);
        }

        // now write the image out for visualization
        char fileName[1000] = { 0 };
        if (argc > 1)
        {
            sprintf (fileName, "%s/", argv[1]);
        }
        strcat(fileName, "tutorial_font_1.png");
        writePng<RendererBaseType> (fileName, rBase);

        delete imageBuffer;
    }
    catch (TutorialException& ex)
    {
        printf ("%s\n", ex.getMessage());
        return 1;
    }
    catch (...)
    {
        printf ("Unknown exception detected.\n");
        return 1;
    }

    return 0;
}
