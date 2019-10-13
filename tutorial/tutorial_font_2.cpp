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
#include <agg_conv_stroke.h>
#include <agg_image_accessors.h>
#include <agg_path_storage.h>
#include <agg_pixfmt_rgba.h>
#include <agg_rasterizer_scanline_aa.h>
#include <agg_renderer_base.h>
#include <agg_renderer_scanline.h>
#include <agg_scanline_p.h>
#include <agg_font_freetype.h>
#include <agg_span_allocator.h>
#include <agg_span_image_filter_rgba.h>
#include <agg_span_interpolator_linear.h>

#include "path.h"
#include "writepng.h"

typedef agg::pixfmt_rgba32  PixelFormat;
typedef agg::renderer_base<PixelFormat> RendererBaseType;

template<class Path>
void
drawImage(RendererBaseType& dst, RendererBaseType& src, const agg::trans_affine& matrix, Path& path, agg::filling_rule_e fillRule)
{
    typedef agg::span_interpolator_linear<agg::trans_affine> InterpolatorType;
    typedef agg::image_accessor_clip<PixelFormat> ImageAccessorType;
    typedef agg::span_image_filter_rgba_nn<ImageAccessorType, InterpolatorType> SpanGeneratorType;
    typedef agg::span_allocator<agg::rgba8> SpanAllocatorType;

    const agg::rgba8 transparentWhiteColor (0xff, 0xff, 0xff, 0);

    double srcWidth = src.width();
    double srcHeight = src.height();

    // from the matrix, compute the invert matrix.  It will
    // be the lookup of src pixels from the dst location.
    agg::trans_affine invertMatrix (matrix);
    invertMatrix.invert();

    InterpolatorType interpolator (invertMatrix);
    ImageAccessorType ia (src.ren(), transparentWhiteColor);
    SpanGeneratorType sg (ia, interpolator);

    agg::rasterizer_scanline_aa<> ras;
    agg::scanline_p8 scanline;
    SpanAllocatorType sa;

    ras.reset();
    ras.filling_rule(fillRule);
    ras.add_path(path);
    agg::render_scanlines_aa(ras, scanline, dst, sa, sg);
}

void
genTextPath (const char* str,
             double x,
             double y,
             bool kerning,
             agg::font_engine_freetype_int32& fontEngine,
             agg::font_cache_manager<agg::font_engine_freetype_int32>& fontCache,
             agg::rgba8 color,
             agg::path_storage& path)
{
    bool first = true;

    // these three are only needed for outline rendering
    agg::conv_curve<agg::font_engine_freetype_int32::path_adaptor_type> curves(fontCache.path_adaptor());

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
                case agg::glyph_data_outline:
                {
                    path.concat_path(curves, 0);
                    break;
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

        // hard code path for simplicity.
        char fontPath[] = "../fonts/DejaVuSerif.ttf";
        double fontSize = 80;
        bool fontHint = true;
        bool fontKerning = true;

        if (!fontEngine.load_font(fontPath, 0, agg::glyph_ren_outline))
        {
            throw TutorialException ("Font is not found.");
        }
        char text[] = "Hello World!";

        agg::path_storage path;
        // enable font hint
        fontEngine.hinting(fontHint);
        // flip y since the image coordinate system has the origin at
        // top left corner and y axis is flipped
        fontEngine.flip_y(true);
        // set font size
        fontEngine.height(fontSize);

        // generate a bunch of horizontal blue lines
        {
            for (int y = 5; y < 100; y += 10)
            {
                double line[] =
                {
                    (double)0, (double)y,
                    (double)imageWidth, (double)y
                };
                SimplePath path (line, sizeof(line) / sizeof(double));
                agg::conv_stroke<SimplePath> strokePath (path);

                double strokeWidth = 5.0;
                strokePath.width(strokeWidth);
                strokePath.line_cap(agg::square_cap);
                strokePath.line_join(agg::miter_join);
                strokePath.miter_limit(strokeWidth);

                ras.reset();
                ras.add_path(strokePath);

                agg::render_scanlines_aa_solid(ras, scanline, rBase, blueColor);
            }
        }

        // draw stripped Hello World!
        {
            genTextPath (text,
                         10, 180,
                         fontKerning,
                         fontEngine,
                         fontCache,
                         blueColor,
                         path);
            agg::trans_affine matrix;
            matrix.translate(0, 100);

            drawImage (rBase, rBase, matrix, path, agg::fill_non_zero);
        }

        // draw a hallow Hello World!
        {
            agg::path_storage path;

            // outter box
            path.move_to(0, 200);
            path.line_to(imageWidth, 200);
            path.line_to(imageWidth, 300);
            path.line_to(0, 300);
            path.line_to(0, 200);

            genTextPath (text,
                         10, 280,
                         fontKerning,
                         fontEngine,
                         fontCache,
                         blueColor,
                         path);
            agg::trans_affine matrix;
            matrix.translate(0, 200);

            drawImage (rBase, rBase, matrix, path, agg::fill_even_odd);
        }

        // now write the image out for visualization
        char fileName[1000] = { 0 };
        if (argc > 1)
        {
            sprintf (fileName, "%s/", argv[1]);
        }
        strcat(fileName, "tutorial_font_2.png");
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
