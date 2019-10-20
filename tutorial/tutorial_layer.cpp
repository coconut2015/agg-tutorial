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
#include <agg_conv_stroke.h>
#include <agg_image_accessors.h>
#include <agg_pixfmt_rgba.h>
#include <agg_rasterizer_scanline_aa.h>
#include <agg_renderer_base.h>
#include <agg_renderer_scanline.h>
#include <agg_scanline_p.h>
#include <agg_span_allocator.h>
#include <agg_span_image_filter_rgba.h>
#include <agg_span_interpolator_linear.h>

#include "path.h"
#include "writepng.h"

typedef agg::pixfmt_rgba32  PixelFormat;
typedef agg::renderer_base<PixelFormat> RendererBaseType;

typedef agg::pixfmt_rgba32_pre  PrePixelFormat;
typedef agg::renderer_base<PrePixelFormat> PreRendererBaseType;

template<class RBaseType1, class RBaseType2>
void
drawImage(RBaseType1& dst, RBaseType2& src, int x, int y, int width, int height)
{
    typedef agg::span_interpolator_linear<agg::trans_affine> InterpolatorType;
    typedef agg::image_accessor_clip<typename RBaseType2::pixfmt_type> ImageAccessorType;
    typedef agg::span_image_filter_rgba_nn<ImageAccessorType, InterpolatorType> SpanGeneratorType;
    typedef agg::span_allocator<agg::rgba8> SpanAllocatorType;

    const agg::rgba8 transparentWhiteColor (0xff, 0xff, 0xff, 0);

    double srcWidth = src.width();
    double srcHeight = src.height();

    // compute the matrix that would transform from the src
    // to the dst drawing area
    agg::trans_affine matrix;
    matrix.scale(width / srcWidth, height / srcHeight);
    matrix.translate(x, y);

    // from the matrix, compute the invert matrix.  It will
    // be the lookup of src pixels from the dst location.
    agg::trans_affine invertMatrix (matrix);
    invertMatrix.invert();

    InterpolatorType interpolator (invertMatrix);
    ImageAccessorType ia (src.ren(), transparentWhiteColor);
    SpanGeneratorType sg (ia, interpolator);

    double rect[] =
    {
        (double)x, (double)y,
        (double)x + width, (double)y,
        (double)x + width, (double)y + height,
        (double)x, (double)y + height,
        (double)x, (double)y
    };

    // The target region to draw the image.
    SimplePath path(rect, sizeof(rect) / sizeof(double));

    agg::rasterizer_scanline_aa<> ras;
    agg::scanline_p8 scanline;
    SpanAllocatorType sa;

    ras.reset();
    ras.add_path(path);
    agg::render_scanlines_aa(ras, scanline, dst, sa, sg);
}


int
main (int argc, const char* argv[])
{
    try
    {
        const int imageWidth = 400;
        const int imageHeight = 400;


        const int pixelSize = PixelFormat::pix_width;

        agg::rendering_buffer   renderBuffer;
        PixelFormat             pixFmt;
        RendererBaseType        rBase;

        unsigned char *imageBuffer = new unsigned char[imageWidth * imageHeight * pixelSize];

        // Hook everything up
        renderBuffer.attach (imageBuffer, imageWidth, imageHeight, imageWidth * pixelSize);
        pixFmt.attach(renderBuffer);
        rBase.attach(pixFmt);

        const agg::rgba8 transparentWhiteColor (0xff, 0xff, 0xff, 0);
        const agg::rgba8 greenColor (0, 0xff, 0, 0x7f);
        const agg::rgba8 redColor (0xff, 0, 0, 0x7f);

        // clear the buffer with transparent white color
        rBase.clear(transparentWhiteColor);

        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_p8 scanline;

        ras.auto_close(false);

        // draw a red semi-transparent square in the middle
        {
            double centerSquare[] =
            {
              100, 100,
              300, 100,
              300, 300,
              100, 300,
              100, 100
            };

            SimplePath path (centerSquare, sizeof(centerSquare) / sizeof(double));

            ras.reset();
            ras.add_path(path);

            agg::render_scanlines_aa_solid(ras, scanline, rBase, redColor);
        }

        double square[] =
        {
           50,  50,
          150,  50,
          150, 150,
           50, 150,
           50,  50
        };

        SimplePath path (square, sizeof(square) / sizeof(double));

        // draw a green semi-transparent square at top left
        {
            ras.reset();
            ras.add_path(path);

            agg::render_scanlines_aa_solid(ras, scanline, rBase, greenColor);
        }

        // draw a layer without PRE at top right
        {
            agg::rendering_buffer   layerBuffer;
            PixelFormat             layerPixFmt;
            RendererBaseType        layerRBase;

            unsigned char *layerImageBuffer = new unsigned char[200 * 200 * pixelSize];

            // Hook everything up
            layerBuffer.attach (layerImageBuffer, 200, 200, 200 * pixelSize);
            layerPixFmt.attach(layerBuffer);
            layerRBase.attach(layerPixFmt);

            double square[] =
            {
               50,  50,
              150,  50,
              150, 150,
               50, 150,
               50,  50
            };

            SimplePath path (square, sizeof(square) / sizeof(double));

            ras.reset();
            ras.add_path(path);

            agg::render_scanlines_aa_solid(ras, scanline, layerRBase, greenColor);

            drawImage (rBase, layerRBase, 200, 0, 200, 200);
        }

        // draw a PRE layer at bottom left
        {
            agg::rendering_buffer   layerBuffer;
            PrePixelFormat          layerPixFmt;
            PreRendererBaseType     layerRBase;

            unsigned char *layerImageBuffer = new unsigned char[200 * 200 * pixelSize];

            // Hook everything up
            layerBuffer.attach (layerImageBuffer, 200, 200, 200 * pixelSize);
            layerPixFmt.attach(layerBuffer);
            layerRBase.attach(layerPixFmt);

            double square[] =
            {
               50,  50,
              150,  50,
              150, 150,
               50, 150,
               50,  50
            };

            SimplePath path (square, sizeof(square) / sizeof(double));

            ras.reset();
            ras.add_path(path);

            agg::render_scanlines_aa_solid(ras, scanline, layerRBase, greenColor);

            drawImage (rBase, layerRBase, 0, 200, 200, 200);
        }

        // now write the image out for visualization
        char fileName[1000] = { 0 };
        if (argc > 1)
        {
            sprintf (fileName, "%s/", argv[1]);
        }
        strcat(fileName, "tutorial_layer.png");
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
